import socket
from pygame import mixer
import serial
import sys
import time
import random

UDP_IP = "172.20.10.4"  # The IP that is printed in the serial monitor from the ESP32
SHARED_UDP_PORT = 4210
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
sock.connect((UDP_IP, SHARED_UDP_PORT))


interacted = [0, 0]
ready_to_guess = 0
planets = ["mercury", "venus", "mars", "jupiter",
           "saturn", "uranus",  "neptune", "pluto"]
fun_facts = []
wifi_num = 0
has_started = 0
r = range(0, 3)


def start_music(i):
    global songs
    mixer.init()
    mixer.music.load(songs[i])
    mixer.music.set_volume(0.7)
    mixer.music.play()
    while True:
        pass


def stop_music():
    if mixer.music.get_busy():
        mixer.music.stop()


def reset():
    pass


def loop():
    wifi_num = 0
    message_showing = 0
    planet = ""
    lit_up = 0

    while True:
        data = sock.recv(2048)
        d = data.decode('utf-8')
        print("d: ", d)

        # light
        if d == '1' and lit_up == 0:
            time.sleep(1)
            light_message()
            start_music(random.randint(0, 3))
            lit_up = 1
            if message_showing == 0:
                keep_exploring_message()

        elif d == '2' and message_showing == 0:
            time.sleep(1)
            touch_message()
            message_showing = 1
            if lit_up == 0:
                keep_exploring_message()

        elif d[0] == 'P':
            print(planets[int(d[1])])

        elif lit_up == 1 and message_showing == 1 and len(planet) > 1:
            get_answer()

        else:
            reset()


def intro():
    print("Welcome!\n")
    print("You have encountered rare, unidentified fallen art. Two days ago this object fell from what was believed to be an alien spacecraft. It's up to you to figure out where it came from.")


def light_message():
    print("Wow! You got the unidentified fallen object to light up! I wonder if the color has anything to do with where it came from...")


def touch_message():
    print("It looks like the ETs sent you a message. Any idea what it means?")


def keep_exploring_message():
    print("Keep exploring the object. There might be more you can learn from it.")


def get_answer():
    ready_to_guess = 1
    ans = input(
        "Which 'planet' do you believe the ETs who dropped this object originated from?")
    if ans == planet:
        print(
            "That is correct! Thank you for helping us learn about extraterrestrial life!")
    else:
        print("That is wrong. The ETs who dropped this came from ",
              planets[planet])
        print(fun_facts[planet])


if __name__ == "__main__":
    global songs
    songs = ['songs/imperial_march.wav', 'songs/starwars.wav', 'songs/et.wav']
    global name
    intro()
    name = input(
        'Please type your name so that you can begin exploring the object.')
    sock.send('Hello ESP32'.encode())
    loop()
