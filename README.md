RC4_RasPi_UART
==============

Implementation of RC4 Algorithm between two Raspberry Pi Modules using UART Communication

To enable the UART Connection enter "sudo nano /boot/cmdline.txt"

We are going to change ttyAMA0 default mode to our own custom mode.

Before editing it looks like :

dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p6 rootfstype=ext4 elevator=deadline rootwait

After Editing it looks like:

dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p6 rootfstype=ext4 elevator=deadline rootwait

enc.c file contains the Encryption of RC4 along with UART Transmission

dcr.c file contains the Decryption of RC4 along with UART Reception
