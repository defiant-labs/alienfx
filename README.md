---------
2012-03-11
* Development on this module is temporarily suspended. I'm simply working more on another machine. I generally plan to get back to working on this module, but anyone else should feel free to grab it and use it as a good starting point for AlienFX on there linux laptops.
---------

alienfx - Linux kernel module
Armando Di Cianno <armando@dicianno.org>

There are a handful of userspace projects that implement support for older Alienware light color hardware, and some that are effectively closed source / "forum software". This module will implement a sysfs based interface for each "zone" of the alienfx device, mimicking the official Windows-based Alienware AlienFX suite.

Supported hardware:
* M11Xr3 (The laptop I own)

Given some info, the following should be supported:
* M11Xr2
* M11X

Given some testers with other Alienware models, we can likely get the module working on all AlienFX type systems.
