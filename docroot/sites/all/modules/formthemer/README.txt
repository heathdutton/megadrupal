This module is a helper for gaining more control over theming of forms.
While most of the elements can be altered or worked-around using #attributes, #prefix and #suffix properties, there are two areas where Drupal does harcode some html in its theme functions.

On the $form itself, where it does force a child bare div (@see #1822210: Investigate removing the inner div in form.html.twig, #495480: Add class to wrapper div of form elements theme_form() or #2250381: Remove the inner div wrapper from forms on why).
On each visible element, where it does add a wrapper div on which you have no control.
The only way to gain control other these is to implement THEME_form and THEME_form_element to override default.

All this module does is implement them on your behalf, and expose additional properties to the form and element to make it more flexible and avoid having to do the logic of preprocessing on which form or element to apply what markup.

These additional properties are all grouped under a #formthemer parent, and vary slightly between $form and $element.
They are obviously all optional and fallback to Drupal's default.

Form

Available properties are:

no_inner_wrapper (bool), set this to true to get rid of the inner div enterly.
inner_wrapper (array), with the following keys:
html_element : element to use as inner wrapper instead of the div.
attributes: (array) standard attributes (class, etc) as used by drupal_attributes(), applied to the wrapper.
Examples, in your form generator function or in form_alter:

Original output:

<form accept-charset="UTF-8" id="contact-site-form" method="post" action="/contact" class="contact-form">
  <div>
    <div class="form-item form-type-textfield form-item-name">
      <label for="edit-name">Your name <span title="This field is required." class="form-required">*</span></label>
      <input type="text" class="form-text required" maxlength="255" size="60" value="" name="name" id="edit-name">
    </div>
    <!--[...]-->
  </div>
</form>
No wrapper:

<?php
$form['#formthemer']['no_inner_wrapper'] = TRUE;
?>

<form accept-charset="UTF-8" id="contact-site-form" method="post" action="/contact" class="contact-form">
  <div class="form-item form-type-textfield form-item-name">
    <label for="edit-name">Your name <span title="This field is required." class="form-required">*</span></label>
    <input type="text" class="form-text required" maxlength="255" size="60" value="" name="name" id="edit-name">
  </div>
  <!--[...]-->
</form>
Custom element:

<?php
$form['#formthemer']['inner_wrapper'] = array(
  'html_element' => 'fieldset',
  'attributes' => array(
    'class' => array('my-first-class', 'my-second-class'),
    'data-myAttribute' => 'myCustomValue',
  ),
);
?>

<form accept-charset="UTF-8" id="contact-site-form" method="post" action="/contact" class="contact-form">
  <fieldset class="my-first-class my-second-class" data-myattribute="myCustomValue">
    <div class="form-item form-type-textfield form-item-name">
      <label for="edit-name">Your name <span title="This field is required." class="form-required">*</span></label>
      <input type="text" class="form-text required" maxlength="255" size="60" value="" name="name" id="edit-name">
    </div>
    <!--[...]-->
  </fieldset>
</form>
Form elements
Available properties are:

no_wrapper (bool), set this to true to get rid of the inner div enterly.
wrapper (array), with the following keys:
html_element: element to use as wrapper instead of the div.
attributes: (array) standard attributes (class, etc) as used by drupal_attributes(), applied to the wrapper.Default is to add them to the standard Drupal ones, not to replace them (see below).
no_default_classes (bool): remove standard drupal classes. If some are given in the 'attributes' property, your custom ones will still be added. Be aware that some modules' js/css might expect those.
Examples, in your form generator function or in form_alter:

Original output:

<div class="form-item form-type-textfield form-item-name">
  <label for="edit-name">Your name <span title="This field is required." class="form-required">*</span></label>
  <input type="text" class="form-text required" maxlength="255" size="60" value="" name="name" id="edit-name">
</div>
No wrapper:

<?php
$form['name']['#formthemer']['no_wrapper'] = TRUE;
?>

<label for="edit-name">Your name <span title="This field is required." class="form-required">*</span></label>
<input type="text" class="form-text required" maxlength="255" size="60" value="" name="name" id="edit-name">
Custom element:

<?php
$form['name']['#formthemer']['wrapper'] = array(
  'html_element' => 'li',
  'attributes' => array(
    'class' => array('my-first-class', 'my-second-class'),
    'data-myAttribute' => 'myCustomValue',
  ),
);
?>

<li data-myattribute="myCustomValue" class="my-first-class my-second-class form-item form-type-textfield form-item-name">
  <label for="edit-name">Your name <span title="This field is required." class="form-required">*</span></label>
  <input type="text" class="form-text required" maxlength="255" size="60" value="admin" name="name" id="edit-name">
</li>
Custom element, no default classes:

<?php
$form['name']['#formthemer']['wrapper'] = array(
  'html_element' => 'li',
  'attributes' => array(
    'class' => array('my-first-class', 'my-second-class'),
    'data-myAttribute' => 'myCustomValue',
  ),
  'no_default_classes' => TRUE,
);
?>

<li data-myattribute="myCustomValue" class="my-first-class my-second-class">
  <label for="edit-name">Your name <span title="This field is required." class="form-required">*</span></label>
 <input type="text" class="form-text required" maxlength="255" size="60" value="admin" name="name" id="edit-name">
</li>
Support for additional tweaks "might" be added, like customization of labels and descriptions, but it probably (hopefully) will die with drupal 7.