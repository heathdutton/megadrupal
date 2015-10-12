BUILDING A THEME WITH FORTY TWO
-------------------------------

The FortyTwo theme is designed to be extended by a sub-theme. You shouldn't modify the
any of the CSS or PHP files in the fortytwo/ folder; instead create a
sub-theme which is located out side of the root fortytwo/ folder.

We tried to make the base theme as clean and simple as possible. It has some
styling for basic input fields and buttons. 


1. Setup the location for your new sub-theme.

    Copy the STARTERKIT folder out of the fortytwo/ folder and rename it to be your
    new sub-theme. IMPORTANT: The name of your sub-theme must start with an
    alphabetic character and can only contain lowercase letters, numbers and
    underscores.

    For example, copy the sites/all/themes/fortytwo/STARTERKIT folder and rename it
    as sites/all/themes/foo.

      Why? Each theme should reside in its own folder. To make it easier to
      upgrade FortyTwo, sub-themes should reside in a folder separate from the base
      theme.

2. Setup the basic information for your sub-theme.

    In your new sub-theme folder, rename the STARTERKIT.info.txt file to include
    the name of your new sub-theme and remove the ".txt" extension. Then edit
    the .info file by editing the name and description field.

    For example, rename the foo/STARTERKIT.info file to foo/foo.info. Edit the
    foo.info file and change "name = Forty Two Sub-theme Starter Kit". Do the same 
    with the description property.

2. Configuration of your subtheme

    A lot of settings can be done on the sub-theme appearance page. CSS/SASS specific 
    settings can be set in /static/sass/_settings.sass. 
    It is possible to compile the SASS files with Compass, there is a config.rb 
    file included. We have not made use of Compass, but if you want to use it, you can. 

3. Responsive?

    By default Forty Two theme has 5 separate layouts; 
    desktop, tablet-landscape, tablet-portrait, mobile-landscape, mobile-portrait. 
    The media queries are defined in the /static/sass/_settings.sass file. 

    You can enable the responsive layout on the admin/appearance page. There are two 
    types of responsive behaviour you can select; adaptive and fluid. 
             
      - Adaptive means that the layout will 'snap' to the next when media 
        queries match. Everything is calculated in pixels.

      - Fluid means that everything is calculated in percentages, every element 
        will be kept in the same spatial weighting, no 'snapping' will occur. 

    The grid-system used is loosly based on the 978 grid-system (http://978.gs/). The 
    grid-configuration for each layout can be modified in the corresponding sass files 
    which are located in the grid folder: /static/sass/base/grid/.

    At the top of each _grid-[..] sass file you can define the number of columns and the
    column and gutter sizes. Only the desktop version (_grid-1280.sass) inherits its 
    column configuration from the /static/sass/_settings.sass file. You can change it there.

    *** IMPORTANT NOTE ***
        Giving elements the proper width based on column numbers should be done in these files.

    There are two ways for column-based calculations, with the mixin 'span-columns' or with
    the function 'calc-grid'. The span-columns mixin will satisfy most of the time. When using
    the fluid grid you need to use 'span-fluid-columns' or 'calc-fluid-grid'. 
    More info on how these mixins/functions work you can check the comments in the 
    /static/sass/lib/_mixins.sass file.

4. Javascript

    There are some libraries and polyfills provided that can be switch on/off. 
