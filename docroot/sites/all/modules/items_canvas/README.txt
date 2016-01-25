Items Canvas Module was written by Brice GATO :

Brice GATO <gato84b [at] google.com>

INSTALLATION
==============================================================================

- You do not need to download any library to run the animation!
- Copy the whole items_canvas directory to your modules directory
   (e.g. DRUPAL_ROOT/sites/all/modules) and activate the module Items Canvas.
- That's all!

EXAMPLE
==============================================================================

* HTML

  <ul>
    <li><a href="http://domain.com/foo1">foo1</a></li>
    <li><a href="http://domain.com/foo2">foo2</a></li>
    <li><a href="http://domain.com/foo3">foo3</a></li>
  </ul>

* With Items Canvas Module

  $items = array(
    0 => array(
            'url'  => 'http://domain.com/foo1',
            'name' => 'foo1',
          ),
    1 => array(
            'url'  => 'http://domain.com/foo2',
            'name' => 'foo2',
          ),
    2 => array(
            'url'  => 'http://domain.com/foo3',
            'name' => 'foo3',
          ),
  );

  print items_canvas_show($items);
