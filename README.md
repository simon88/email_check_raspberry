# Email checker for raspberry

This simple project makes it possible to check if a new mail is waiting in an inbox, if yes then a led is lit on a port gpio of the raspberry.

This project consists of two parts :
- Client side, this part that will take care to check if a new mail is waiting. Also allows access to the module to turn on the LED. User space to kernel space.
- Kernel side, this kernel module allows to manage the gpio port of the raspberry according to the information received from the user space.

## How to build

Each parts contain Makefile to build. Is provided for cross compilation, you will need to adapt KDIR and CROSS_COMPILE to your computer.

About the kernel module Makefile, this Makefile

Finally just type (in client and driver directory) :

```
make
```

## Load kernel module

Once the module compile it is enough to load it in the raspberry, to do it

```
sudo insmod email_check.ko
```

It is also necessary to give the rights of reading and writing on the file

```
sudo chmod 666 /dev/email_check
```

If you want unload module type :

```
sudo rmmod email_check
```

This is all the module is loaded is ready to be used

## Config client side

This program is an infinite loop that checks the mailbox every minute to find out if a new mail has arrived. If a new mail is in INBOX the LED lights up for 10 seconds

When you start the program you must fill in the email address and the password associated with the account and the imap address of the server. The program offers default imap addresses like google for example.

For Google :
> You must allow imap in your account

## Electronic side

It is very simple you need a led and a resistance of 270 ohm. In the original code the raspberry output that is used is pin 24. And for GND you can use pin 3.





