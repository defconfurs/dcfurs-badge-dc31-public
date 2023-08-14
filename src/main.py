
from machine import Timer, Pin
from micropython import schedule
import math
import time
from random import random
import array
import gc
from rp2 import PIO, StateMachine, asm_pio


from touch import TouchController
from is31fl3737 import is31fl3737, rgb_value


TOUCH_PINS = (4, 5, 6, 7, 22, 23, 24, 25)
TOUCH_OVERRIDE_TIME = 30
bars = ['⠀', '⡀', '⣀', '⣄', '⣤', '⣦', '⣶', '⣷', '⣿']


def pallet_rainbow(target):
    for i in range(len(target)):
        target[i][0] = i/len(target)
        target[i][1] = 1.0
        target[i][2] = 255

def pallet_set_colour(target, hue, hue_spread, sat, sat_spread):
    offset = 0.0
    start = 0
    end   = 0
    count = int(len(target)/16)
    for i in range(16):
        dir_change = random() * 0.1
        if offset + dir_change >  hue_spread/2:
            dir_change = -dir_change
        if offset - dir_change < -hue_spread/2:
            dir_change = -dir_change
        offset += dir_change
        if offset >  hue_spread/2:
            offset =  hue_spread/2
        if offset < -hue_spread/2:
            offset = -hue_spread/2
        
        end   = offset
        step_size = (start - end)/count
        for j in range(count):
            pos = j / count
            offset = (step_size * pos) + start
            target[i*count + j][0] = hue+offset
            target[i*count + j][1] = sat
            target[i*count + j][2] = 255
        start = end
    end   = 0
    step_size = (start - end)/count
    for j in range(count):
        pos = j / count
        offset = (step_size * pos) + start
        target[len(target)-count+j][0] = hue+offset
        target[len(target)-count+j][1] = sat
        target[len(target)-count+j][2] = 255

def pallet_blue(target):
    pallet_set_colour(target, 0.5, 0.3, 0.8, 0.4)

def pallet_red(target):
    pallet_set_colour(target, 0.75, 0.3, 0.8, 0.4)

def pallet_green(target):
    pallet_set_colour(target, 0.0, 0.3, 0.8, 0.4)

def pallet_purple(target):
    pallet_set_colour(target, 0.25, 0.3, 0.8, 0.4)

class animation_rainbow_around:
    def __init__(self, badge):
        self.badge = badge
        self.offset = 0.0
    
    def update(self):
        self.offset += 0.5
        for i in range(46):
            self.badge.disp.clockwise[i].hsv(self.badge.pallet[int(1024*((i+self.offset)/60))&0x3FF][0], 1.0, 100)
        self.badge.disp.eye1.hsv(self.badge.pallet[int(1024*((i+self.offset)/46))&0x3FF][0], 1.0, 200)
        self.badge.disp.eye2.hsv(self.badge.pallet[int(1024*((i+self.offset)/46))&0x3FF][0], 1.0, 200)

class animation_rainbow_down:
    def __init__(self, badge):
        self.badge = badge
        self.offset = 0.0
    
    def update(self):
        self.offset -= 0.5
        for i in range(48):
            self.badge.disp.downward[i].hsv(self.badge.pallet[int(1024*(i+self.offset)/100)&0x3FF][0], 1.0, 100)

class animation_chasers:
    def __init__(self, badge):
        self.badge = badge
        self.traces = []
        self.last = time.ticks_ms()
        self.next = self.last + int(random() * 1000)
        self.max_traces = 3
        self.decay = 0.95
        self.brightness = 255
        self.buffer = [rgb_value() for i in range(46)]
        self.eye_offset = 0.0

    def update(self):
        if time.ticks_ms() > self.next:
            colour = self.badge.pallet[int(1024*random())][0]
            speed = (0.5-((random()-0.5)**2))*1.2*(1 if (random()>0.5) else 0)
            
            #                      0      1               2            3            4         5
            #                   colour  speed           position    life_rate      life      record 
            self.traces.append([colour, speed, float(random()*46), (random()*0.07), 1.0,  array.array("f", [0]*46)])
            self.next = time.ticks_ms() + int(random()*3500)
        
        for trace in self.traces:
            if trace[4] <= 0:
                self.traces.remove(trace)

        for i in range(46):
            self.buffer[i].r = 0.0
            self.buffer[i].g = 0.0
            self.buffer[i].b = 0.0

        for trace in self.traces:
            trace[4] -= trace[3]
            if trace[4] < 0:
                trace[4] = 0
            if trace[4] > 0.1:
                end_gain = 1.0
            else:
                end_gain = trace[4] * 10.0
            for i in range(46):
                trace[5][i] *= (self.decay * end_gain)
            
            trace[2] += trace[1]
            trace[5][int(trace[2])%46] = self.brightness * end_gain

            for i in range(46):
                r,g,b = self.buffer[0].hsv(trace[0],1.0,trace[5][i],1.0,ret_value=True)
                self.buffer[i].value[0] += r
                self.buffer[i].value[1] += g
                self.buffer[i].value[2] += b

        for i in range(46):
            if self.buffer[i].r > self.brightness:
                self.buffer[i].r = self.brightness
            if self.buffer[i].g > self.brightness:
                self.buffer[i].g = self.brightness
            if self.buffer[i].b > self.brightness:
                self.buffer[i].b = self.brightness
            if self.buffer[i].r < 0:
                self.buffer[i].r = 0
            if self.buffer[i].g < 0:
                self.buffer[i].g = 0
            if self.buffer[i].b < 0:
                self.buffer[i].b = 0
            self.badge.disp.clockwise[i].value[0] = int(self.buffer[i].r)
            self.badge.disp.clockwise[i].value[1] = int(self.buffer[i].g)
            self.badge.disp.clockwise[i].value[2] = int(self.buffer[i].b)

        self.eye_offset += 0.5
        self.badge.disp.eye1.hsv((i+self.eye_offset)/46, 1.0, self.brightness)
        self.badge.disp.eye2.hsv((i+self.eye_offset)/46, 1.0, self.brightness)
        
class animation_blush_override:
    def __init__(self, badge):
        self.badge = badge
        self.timeout = 0

    def reset(self, timeout):
        self.timeout = timeout
    
    def update(self):
        if self.timeout <= 0:
            return
        self.timeout -= 1

        for l in self.badge.disp.cheak1:
            l.hsv(0, 1.0, 255.0)
        for l in self.badge.disp.cheak2:
            l.hsv(0, 1.0, 255.0)


class animation_ear1_override:
    def __init__(self, badge):
        self.badge = badge
        self.timeout = 0

    def reset(self, timeout):
        self.timeout = timeout
    
    def update(self):
        if self.timeout <= 0:
            return
        self.timeout -= 1

        for l in self.badge.disp.ear1:
            l.hsv(0, 1.0, 255.0)


class animation_ear2_override:
    def __init__(self, badge):
        self.badge = badge
        self.timeout = 0

    def reset(self, timeout):
        self.timeout = timeout
    
    def update(self):
        if self.timeout <= 0:
            return
        self.timeout -= 1

        for l in self.badge.disp.ear2:
            l.hsv(0, 1.0, 255.0)


class badge(object):
    def __init__(self):
        self.disp = is31fl3737()
        self.anim_index = 1
        self.half_bright = False
        self.animations = [animation_rainbow_around(self),animation_rainbow_down(self),animation_chasers(self)]
        self.pallet_index = 0
        self.pallet_functions = [pallet_rainbow, pallet_blue, pallet_red, pallet_green, pallet_purple]

        self.sw4 = Pin(10)
        self.sw5 = Pin(11)

        self.sw4_state = 0xFF
        self.sw5_state = 0xFF

        self.sw4_count = 0
        self.sw5_count = 0
        self.sw4_last  = 0
        self.sw5_last  = 0

        self.touch = TouchController(TOUCH_PINS)
        self.blush_override = animation_blush_override(self)
        self.ear1_override = animation_ear1_override(self)
        self.ear2_override = animation_ear2_override(self)
        self._pre_rub_brightness = 255
        self.chin_rub_timeout = 0

        self.pallet = [[0.0,0.0,0.0] for i in range(1024)]
        self.pallet_functions[self.pallet_index](self.pallet)

        print("Dreams are messages from the deep.")
        self.timer = Timer(mode=Timer.PERIODIC, freq=15, callback=self.isr_update)

    def isr_update(self,*args):
        schedule(self.update, self)

    def update(self,*args):
        self.touch.update()

        if (self.touch.channels[0].level + self.touch.channels[1].level) > 0.75:
            self.blush_override.reset(timeout=TOUCH_OVERRIDE_TIME)
        if (self.touch.channels[3].level) > 0.75:
            self.ear1_override.reset(timeout=TOUCH_OVERRIDE_TIME // 2)
        if (self.touch.channels[2].level) > 0.75:
            self.ear2_override.reset(timeout=TOUCH_OVERRIDE_TIME // 2)

        rub_power = sum(_.level for _ in self.touch.channels[4:])
        if rub_power > 0.75:
            if self.chin_rub_timeout == 0:
                self.disp.brightness = 20
            self.disp.brightness = int(min(self.disp.brightness + (rub_power*5), 255))
            self.chin_rub_timeout = time.ticks_ms() + 2000

        print('\rTouch: ', end='')
        for i, c in enumerate(self.touch.channels):
            print(f'   {TOUCH_PINS[i]}:{bars[min(len(bars)-1, int(c.level * len(bars)))]}', end='')
        print(f'    rp: {rub_power} br: {self.disp.brightness}', end='')

        self.sw4_state <<= 1
        self.sw4_state |= self.sw4()
        self.sw5_state <<= 1
        self.sw5_state |= self.sw5()
        if (self.sw4_state & 0x3) == 0x0:
            self.sw4_count += 1
        else:
            self.sw4_count = 0
        if (self.sw5_state & 0x3) == 0x0:
            self.sw5_count += 1
        else:
            self.sw5_count = 0
        
        if self.sw4_count == 0 and self.sw4_last > 0:
            if self.sw4_last > 10: # long press
                self.half_bright = not self.half_bright
                if self.half_bright:
                    self.disp.brightness = 50
                else:
                    self.disp.brightness = 255                
            else:
                self.anim_index += 1
                if self.anim_index >= len(self.animations): 
                    self.anim_index = 0
        elif self.sw5_count == 0 and self.sw5_last > 0:
            if self.sw5_last > 10:
                self.pallet_index += 1
                if self.pallet_index >= len(self.pallet_functions):
                    self.pallet_index = 0
                self.pallet_functions[self.pallet_index](self.pallet)
            else:
                self.anim_index -= 1
                if self.anim_index < 0:
                    self.anim_index = len(self.animations)-1
        
        self.sw4_last = self.sw4_count
        self.sw5_last = self.sw5_count

        print(f' crto: {self.chin_rub_timeout} t: {time.ticks_ms()}', end='')
        if self.chin_rub_timeout > 0 and time.ticks_ms() >= self.chin_rub_timeout:
            self.disp.brighness = 50 if self.half_bright else 255
            self.chin_rub_timeout = 0

        self.animations[self.anim_index].update()

        # these "overrides" are a quick way of doing this and should be replaced with something better``
        self.blush_override.update()
        self.ear1_override.update()
        self.ear2_override.update()

        self.disp.update()
        gc.collect()

    def run(self):
        while True:
            self.update()
            time.sleep(1/15)


global t
t = badge()
#t.run()

