# LEAF
Language Extending Add-on for Filemaker

Look: Filemaker, like any development tool, has some rough edges that expose themselves when you've worked with it enough.
That's why I developed this extension at work, and have finally decided to put it up on GitHub in the hopes that it 
will be useful to another poor schmuck stuck working on building a complicated Filemaker solution.

Rapid Application Development and Prototyping is the name of the game, and no language gets that done better in my estimation
than Python.  That is why, currently, this plugin only adds support for Python; however, it is written in such a way that
slapping in another language (that supports proper C callbacks) should be a trivial proposition (and hence the project name).

You'll also find in the Docs folder a mirror of some obscure tidbits floating around online that greatly aided my understanding
of the Filemaker API, just so that these don't disappear entirely and leave anyone stuck.  Sure, the default SDK is good, but
you'll find it leaves a lot to be desired if you try to do anything remotely interesting like this.

# How does it work?

If you don't really care about the code, and just want to jump in and do some kewl Python stuff from FM, just take the .fmx
from the Output folder here, and add it to your extensions.  You'll see an external function added called LEAF_ExecutePython.  
Usage is:
  *LEAF_ExecutePython ( [full path to .py]; [script arguments] )*
  
I usually wrap this in a custom function so other developers see something a little more descriptive like *SendBulkEmail*.
To actually call this function from the Calculation Engine, I usually wrap it as a Set Variable value, which will then store your
Python script output.

Now, to actually write your scripts, all your script arguments you passed will get stored in a global FMargs[] array in Python.
Then, you pass values back to FM by simply printing them - so a *print "Hello World"* would pass the Text value *"Hello World"* to FM.
No fancy type transfers here, we're talking str() to Text, so do your conversions internally on either end before passing them back-and-forth.

If all of this made absolutely no sense, just take a look at the Example folder, where you'll find a Sandbox .fmp12, and a Hello World .py!

# Last but not least, a couple of Caveats:

* *This project was tested and builds with a few warnings in Visual Studio 2010 for Windows 7 x64, deployed to Filemaker Pro 13 and 13 Advanced.*  
* *Though other project files are included for XCode and older versions of VS, they were not modified from the files provided by the SDK, so YMMV*
* *Like I said before, this mainly just adds support for basic Python - I only use Python 2.7.6 32-bit at work due to library
   support for things like Twisted, but I've used vanilla 64-bit at home, and vanilla 32-bit 3.4.2 at times; again, YMMV.*
* *Because my employer is fond of Open Source solutions, and holds no countenance over this code, the only licensing restrictions
   you're going to come up against are with that of Filemaker, Inc.  I'd check with them for whatever terms and conditions they
   allow you to do stuff with, since this is an extension of their product, but if you end up getting away with selling something
   utilizing this code, please give me a public shout-out and link this GitHub page in your docs if you could :D*
   
   
**Now get back to work!**
