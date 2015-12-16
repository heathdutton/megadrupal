FooBar Table
===============

Description
-----------

FooBar Table duplicates standard table theme and adds two things.

1. Lets you render tables with <tfoot> tag using theme().
   Element declared like this:
    $build['mytable'] = array(
      '#theme' => 'table',
      '#header' => array('head1', 'head2'),
      '#footer' => array('foot1', 'foot2'),
      '#rows' => array(array(1, 2), array(3, 4)),
    );
    Will be rendered as something like this:
    <table class="sticky-enabled">
        <thead>
            <tr><th>head1</th><th>head2</th></tr>
        </thead>
        <tfoot>
            <tr><td>foot1</td><td>foot2</td></tr>
        </tfoot>
        <tbody>
            <tr class="odd"><td>1</td><td>2</td></tr>
            <tr class="even"><td>3</td><td>4</td></tr>
        </tbody>
    </table>

2. Adds javascript sorting for you table.
    To use javascript sorting you need to install Libraries module, create 
    "jquery.tablesorter" folder in you libraries folder,
    and put jquery.tablesorter.min.js there.

    So element declared like this:
    $build['mytable'] = array(
      '#theme' => 'table',
      '#header' => array(
        'head1',
        array('data' => 'head2', 'sort' => 'asc'),
        array('data' => 'head3', 'sorter' => FALSE)
      ),
      '#footer' => array('foot1', 'foot2', 'foot3'),
      '#rows' => array(array(1, 2, 3), array(4, 5, 6)),
      '#jsorted' => TRUE,
    );
    Will be rendered as a table sorted by second column with click handlers
    attached to header cells for changing sorting order.
    You can use 'sorter' field to pass different sorting data-parser to 
    tablesorter plugin (You might want to sort data not supported by default).
    If you want to disable sorting on a particular column, use FALSE as value
    for 'sorter'.

You can also keep 'table' theme as is in the core and use 'foobartable' when
theming ('#theme' => 'foobartable') with the same result as described above.
Use module's settings page to select on of these options (by default 'table'
theme is replaced)

Dependencies
------------

Libraries - optional


Configuration
-------------

- Download and enable module.
- If you don't want core 'table' theme to be replaced with this module's method
  and want to use both 'table' and 'foobartable' for separate needs you can
  select this option on the settings page:
  admin/config/user-interface/foobartable
