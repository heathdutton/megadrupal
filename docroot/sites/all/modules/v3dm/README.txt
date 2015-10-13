Description
===========

Viewer for 3D Models (v3dm) provides a field type to store 3D model files
and several formatters to visualize them.

The field type doesn't do very much on its own. At present, it's just a
container to upload 3D Models. In turn, v3dm provides integration modules
for 3DHOP, JSC3D and thingiview.js viewers.

This project comes out of the need of a viewer for STL files
(stereolithography format for 3D printers). While there were no specific
requirements for a given player, the desition was to implement it in a
generic way so it could support several players.

Moreover, since available players support a variety of formats, it is
possible to use v3dm to visualize several file types. See the support
matrix below.

A known flaw of the module is that it doesn't enforce to use a valid
formatter(player) for the extensions configured in a field. It's up to the
site builder to guarantee the consistency: ie, choose a set of extensions
you know are recognized by the selected formatter.


Details
=======

Four modules are provided:

 * v3dm: Provides the 3dmodel field type
 * v3dm_hop: Provides a formatter to integrate with 3DHOP
 * v3dm_jscd: Provides a formatter to integrate with JSC3D
 * v3dm_thingiviewjs: Provides a formatter to integrate with Thingiview.js


File format support matrix
--------------------------

This table shows the relation of file formats natively supported by each
of the supported players. Refer to each player's documentation for
details.

|Format/Player  |3DHOP|JSC3D|Thingiview|
|---------------|-----|-----|----------|
| Autodesk 3DS  |     |  x  |          |
| OpenCTM       |     |  x  |          |
| PLY           |  x  |     |          |
| NXS           |  x  |     |          |
| STL           |     |  x  |     x    |
| Wavefront OBJ |     |  x  |     x    |


Installation
============

Obviously each formatter requires the corresponding third-party library.

Libraries may be placed in sites/all/libraries or if you've enabled libraries
module, any valid place such as a profile or a site directory.

v3dm_hop
--------

Homepage: http://vcg.isti.cnr.it/3dhop/
Expected libraries directory: 3dhop

Example instructions:

    cd sites/all/libraries
    wget http://sourceforge.net/projects/tdhop/files/3DHOP%202.0/3DHOP_2.0_Essential.zip/download -O 3dhop-2.0.zip
    unzip 3dhop-2.0-zip
    mv 3DHOP_2.0_Essential/ 3dhop

This is the essential package, but you can still safely remove demos, docs, etc.

    rf -rf 3dhop/{*.pdf,*.html,docs,models}


v3dm_jsc3d
----------

Homepage: http://code.google.com/p/jsc3d/
Expected libraries directory: jsc3d

Example instructions:

    cd sites/all/libraries
    wget http://jsc3d.googlecode.com/files/jsc3d-full-1.6.5.zip
    unzip jsc3d-full-1.6.5.zip -d jsc3d
    rm jsc3d-full-1.6.5.zip

This is a full package, you can safely remove demos, docs, etc.

    rm -rf jsc3d/{bin,build,demos,docs,tools}


v3dm_thingiviewjs
-----------------

Thingiview is originally created by Tony Buser (@tbuser). However the library
is not updated since 2011 and several forks have been created. The one by
Guillaume Seguin (@iXce) seems the better.

v3dm_thingiviewjs module is based on @iXce's fork.

Homepage: https://github.com/tbuser/thingiview.js
@iXce fork: https://github.com/iXce/thingiview.js
Expected libraries directory: thingiview.js

Example instructions:

    cd sites/all/libraries
    wget https://github.com/iXce/thingiview.js/archive/master.zip
    unzip master
    mv thingiview.js-master/ thingiview.js
    rm master.zip

It is *encouraged* to remove php/ subdir to avoid possible security holes, also
examples can be remove to free some disk space.

    rm -rf thingiview.js/{examples,php}


