Examples
========

First, build the shared libraries in this directory.
```
$ make
```

This will create libpulse.so and libtrig.so

Pulse Example
-------------

You can run the pulse test like this:
```
$ LD_LIBRARY_PATH=/usr/local/sandvine/lib /usr/local/sandvine/sandscript_test_pts --enable_svlog --shared_libs . --input ./pulse.conf --run_forever
```

Stop it with ^C when you get bored.


Pulse Example
-------------

You can run the trig test like this:
```
$ LD_LIBRARY_PATH=/usr/local/sandvine/lib /usr/local/sandvine/sandscript_test_pts --enable_svlog --shared_libs . --input ./trig.conf
```

