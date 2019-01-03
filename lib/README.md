Libraries
=========

This directory contains libraries necessary for the project. To keep up with
their development, the concerned repositories are referenced as [submodules][1].
If you ever have troubles building the project, check that this directory is
full -- if you don't use the `--recursive` flag during clone, git won't populate
it by default.

Since QtWebFlix is already included various package, it was decided that all
libraries in this directory will be linked to it *statically* in order not to
pollute the system library directory, and more importantly, to make sure all
conflicts with already installed libraries are avoided. For this reason, custom
*patched* versions of `.pro` files have been generated to superseed default
configuration in the referenced directories.

For more information, see the `.pro` files in this directory and in [src/][2].

[1]: https://git-scm.com/docs/git-submodule
[2]: ../src/src.pro

