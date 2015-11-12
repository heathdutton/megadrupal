VALKYRIE-MOUNT
==============

Using NFS with Vagrant is great, but requires some tweaks to make for a
seemless dev environment. This 'faux' plugin provides said tweaks. Simply
add it to your project (perhaps as a git submodule), and include it in your
Vagrantfile like so:

  require "path/to/valkyrie-mount/mount.rb"


Background
----------

The principle issue with using NFS with Vagrant is that of mapping user and
group IDs between the host machine and the VM. On Linux hosts, for example,
your UID will be `1000`, almost without fail. The way that Vagrant base boxes
are built almost guarantees a collision here, since the 'vagrant' user is
likely to have that same UID in the VM.

This is fine if you want to run file operations as 'vagrant', but if you want
to operate as, say, 'aegir' instead, you're going to run into problems.
`chown`, `chmod` and `chgrp` are all likely to fail for you.

The solution we've found that works best is to re-map the UID and GID of the
user you want to operate as, so that it matches your user's on the host system.
This, of course, presents its own challenges, since you can't change these
attributes of a logged-in user.

Since we use Ubuntu as our principal OS, we first allow SSH for the 'ubuntu'
user, by copying the keys used by the 'vagrant' user. We then log back in via
SSH as the 'ubuntu' user and (using Ansible) we re-map the UID/GID of the user
and group, as appropriate for the host OS. We then close the SSH connection, to
allow regular Vagrant operations to proceed un-disturbed.
