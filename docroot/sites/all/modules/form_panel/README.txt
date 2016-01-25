
Form Panel

This module provides two theme functions which, in turn, provide an easy way to
put multiple input form elements on the same line. At the most basic level, it
can turn this:

  First Name:
  [textfield]
  
  Last Name:
  [textfield]

  Address:
  [textfield]

  City:
  [textfield]

  State:
  [select list]

into this:

  First Name:  Last Name:
  [textfield]  [textfield]

  Address:
  [textfield]

  City:        State:
  [textfield]  [select list]

It can do this using either an HTML table or <div> tags. In both cases, it
includes various CSS classes to help with styling.

It is important to note, however, that this module does nothing on its own. You
must refer to the included themes from PHP code in order for there to be any
effect.


The Two Themes

The theme named form_panel_table provides a table-based output. The theme named
form_panel_div wraps each row in a <div> tag with a specific class, and each
element in its own <div>.

You can choose which theme to use by assigning it to the #theme attribute of the
form element enclosing those you wish to theme. For instance:

  $form = array('#theme' => 'form_panel_table');
  $form['foo'] = ...first form element goes here...
  $form['bar'] = ...second form element goes here...

If you are adding a table or <div> set to an existing form, it can help to put
this into a sub-array:

  $form['my-table'] = array('#theme' => 'form_panel_table');
  $form['my-table']['foo'] = ...first form element goes here...
  $form['my-table']['bar'] = ...second form element goes here...


Positioning

This module uses a grid-based approach to describe where to put form elements.
Rows and columns are numbered starting with 1, not 0, however. This avoids a
conflict with the way Drupal uses element weights, internally. Furthermore, when
using the table theme, row 1 is considered to be the table header; its cells are
always surrounded with <th> tags.

Row and column numbers need not be sequential. You could have row numbers like
2, 10, 99, 101 if you want. The important thing when using the table theme is to
be consistent for each row, because any "missing" values will be subject to
column or row "spans", to take up the gaps. See the section on Omitting
Rows/Columns for more details.

There are four different ways row and column numbers can be represented for this
module. It is important that you not mix methods within a given table or themed
<div>.

- Using #form_panel_row and #form_panel_col

The easiest method to understand is to explicitly set the row and column numbers
using two special attributes that the theme function reads.

In the example that follows, and most of the other examples in this document,
we're using a simple #value to represent the form element. Normally, you would
set #type, #title, #description, and other attributes to define a real form
element that accepts user input.

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1));
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++),
    '#form_panel_row' => 2, '#form_panel_col' => 1);
  $form[chr($i)] = array('#value' => chr($i++),
    '#form_panel_row' => 2, '#form_panel_col' => 2);
  $form[chr($i)] = array('#value' => chr($i++),
    '#form_panel_row' => 3, '#form_panel_col' => 1);
  $form[chr($i)] = array('#value' => chr($i++),
    '#form_panel_row' => 3, '#form_panel_col' => 2);
  // This next line is just an example. Normally, you would "return" the value.
  print drupal_render($form);

- Integer

This is the default method, if #form_panel_row and #form_panel_col are not
specified.
 
In this method, the row and column are represented by setting the #weight of an
element to an integer from 1001 to 999999. The thousands part of the number is
the row, and the part less than 1000 is the column.

Example:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1));
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2001);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2002);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3001);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3002);
  print drupal_render($form);

- Decimal

To use this method, you set the #form_panel_weights_decimal attribute on the
outer form element to TRUE.

In this method, the row and column are represented by setting the #weight of an
element to a decimal number from 1.001 to 999.999. The integer part of the
number is the row, and the fraction (the part to the right of the decimal point)
is the column.

To make the code look cleaner, you can leave off the extra zeros in the
fraction, if you are dealing with few enough columns. For instance, the
rows/columns (1.001, 1.003, 1.007) can also be represented as (1.1, 1.3, 1.7).
That's because, internally, the columns get converted to 100, 300, and 700.

You need to be careful when using this method, however. If you use the weight
1.1 to represent (row=1, col=1), how do you represent (row=1, col=10)? The
numbers 1.1 and 1.10 are identical, as far as PHP is concerned. Instead, you
need to use either 1.01 or 1.001 for (row=1, col=1). Then, you can use either
1.10 or 1.010 for (row=1, col=10).

Example:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1),
    '#form_panel_weights_decimal' => TRUE);
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2.1);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2.2);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3.1);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3.2);
  print drupal_render($form);

- Hexadecimal

To use this method, you set the #form_panel_weights_hex attribute on the outer
form element to TRUE.
 
In this method, the row and column are represented by setting the #weight of an
element to a hexadecimal number from 0x0101 to 0xFFFF. The part of the number >=
256 (decimal) is the row, and part of the number < 256 is the column.

Example:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1),
    '#form_panel_weights_hex' => TRUE);
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 0x0201);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 0x0202);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 0x0301);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 0x0302);
  print drupal_render($form);


Omitting Rows/Columns

When a table is generated using the form_panel_table theme, missing rows and
columns will be merged. This means that appropriate rowspan/colspan attributes
are generated automatically.

But it's not always possible to describe a table accurately using this method,
so some compromises have to be made.

By default, colspans are used to merge each filled cell with any empty cells to
its right. If the row starts with empty cells, they are created first. If the
#form_panel_table_span_rows attribute is TRUE, then this same procedure will be
used to merge cells downward, instead. The exception is the header row, if
present, which is always merged horizontally to the right.

For example, let's say we have some code like this:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1),
    '#form_panel_weights_decimal' => TRUE);
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2.1);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2.2);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3.1);
  print drupal_render($form);

In this case, we've omitted (row=3, col=2), so the generated table looks
something like this:

  +---+---+
  | A | B |
  +---+---+
  | C     |
  +-------+

If, instead, we set #form_panel_table_span_rows to TRUE, we get:

  +---+---+
  | A | B |
  +---+   |
  | C |   |
  +---+---+

If we then change the code to make the first row the header (by reducing its
number to 1), we get:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1),
    '#form_panel_table_span_rows' => TRUE,
    '#form_panel_weights_decimal' => TRUE);
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 1.1);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 1.2);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3.1);

  +---+---+
  | A | B |
  +===+===+
  | C |   |
  +---+---+

To see what happens when missing cells are in the leftmost or top position,
consider this example:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1),
    '#form_panel_weights_decimal' => TRUE);
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2.2);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3.1);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3.2);

  +---+---+
  |   | A |
  +---+---+
  | B | C |
  +---+---+

In this case, the upper-left cell contains the HTML entity "&nbsp;", by default.
This can be changed using the #form_panel_table_filler attribute.

You can, of course, have multiple omitted cells in your table. Depending on the
situation, the browser may choose to ignore some of the spans, however. So the
table may not always look the way you intended.


Reuse of Cells

It is possible to have more than one form element appear in the same table cell
or <div> tag, simply by assigning them the same weight. When form_panel
encounters this situation, it concatenates the form elements within the same
containing tag.

Example:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array('border' => 1));
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2001);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2002);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2002);
  print drupal_render($form);

  +---+----+
  | A | BC |
  +---+----+

The order of elements presented in this way depends on the order Drupal's Forms
API code assigns to the elements. In this case, both the "B" and the "C" have
the same weight, so they are ordered based upon their order of appearance in the
$form array.

So, what if you wanted to reverse the order of "B" and "C" without changing the
way the elements are stored in in the array? Because form_panel only looks at
certain parts of the weight, it is possible to influence the outcome of Drupal's
weight-based sorting. In the case of integer weights, anything following a
decimal point will be ignored by form_panel.

For example, this modified code:

  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2001);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2002.2);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2002.1);

would produce:

  +---+----+
  | A | CB |
  +---+----+

A variation on this method can also be used with #form_panel_weights_decimal. In
this case, the amount you need to add to the weight is 0.0001.

If, instead of using the weights to specify the row and column of your form
elements, you use #form_panel_row and #form_panel_col, then the entire weight
value is used for ordering within an individual cell.


CSS Considerations

If you are using form_panel_div, you will most likely want to create a class for
your inner divs that sets them to "display: inline". Otherwise, they will not
appear on lines by themselves.

The best way to do this using the default definitions is to include these lines
in your theme's CSS file:

  div.form-panel-div {
    display: inline;
  }

Another way to accomplish the same thing, entirely in PHP code, is to add the
style to #form_panel_div_attributes. For example:

  $form = array(
    '#theme' => 'form_panel_div',
    '#form_panel_div_attributes' => array(
      'style' => 'display: inline'),
    );

This latter method is not generally considered good Web design, however.


More Attributes

The two themes also accept a number of other optional parameters, which change
the appearance of the final output.

#form_panel_div_attributes (form_panel_div theme only)

  This array of attributes is added the <div> that wraps each form element. The
  theme also adds its own class, form-panel-div, to the class attribute.

#form_panel_number_cols (both themes)

  If TRUE, a class of the name form-panel-col-NNN, where NNN is from 000 to 999,
  is added to each <div>, <td>, or <th> that wraps each form element. This
  allows you to give each column of the output a different style.

#form_panel_number_rows (both themes)

  If TRUE, a class of the name form-panel-row-NNN, where NNN is from 000 to 999,
  is added to each <div> or <tr> that wraps a row. This allows you to give each
  row of the output a different style.

#form_panel_odd_even (both themes)

  If TRUE, the same two classes "odd" and "even" that are used in other tables
  generated by Drupal are also added to the rows of output this module
  generates. This allows you to give alternating rows a slightly different
  appearance.

#form_panel_table_attributes (form_panel_table only)

  This array of attributes is added the <table> tag. The theme also adds its own
  class, form-panel, to the class attribute.

  Changing the list of attributes other than "class" can give you a quick way to
  control some of the table's appearance (border, cellspacing, etc.) without
  using CSS, though this is generally not considered good practice in modern Web
  design.

#form_panel_table_caption (form_panel_table only)

  This attribute contains an optional caption, which appears at the top of the
  table.

#form_panel_table_filler (form_panel_table only)

  When generating a table with missing cells, it is sometimes necessary to
  include empty ("<td></td>") cells. But some browsers will not render an empty
  cell the same as those with contents, so the value of this attribute will be
  inserted into the cell.

  If not specified, this attribute defaults to the string "&nbsp;".

#form_panel_table_span_rows (form_panel_table only)

  If TRUE, merge missing cells downward, instead of to the right.

#form_panel_td_attributes (form_panel_table only)

  This array of attributes is added the <td> that wraps each form element. The
  theme also adds its own class, form-panel-cell, to the class attribute.

#form_panel_tr_attributes (form_panel_table only)

  This array of attributes is added the <tr> that wraps each form element. The
  theme also adds its own class, form-panel-row, to the class attribute.

An Example Using Attributes:

  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_attributes' => array(
      'border' => 1,
      'class' => 'my-table'),
    );
  $i = ord('A');
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2001);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 2002);
  $form[chr($i)] = array('#value' => chr($i++), '#weight' => 3002);
  print drupal_render($form);
