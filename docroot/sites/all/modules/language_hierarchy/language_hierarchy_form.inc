<?php


/**
 * Build the parent-child Language hierarchy form.
 *
 * @param $form_state
 *   The form state.
 *
 * @return array
 *   A form array set for theming by theme_language_hierarchy_form()
 *
 */
function language_hierarchy_form($form_state) {
  // Identify that the elements in 'language_items' are a collection, to
  // prevent Form API from flattening the array when submitted.
  $form['language_items']['#tree'] = TRUE;

  // Tabledrag will take care of updating the parent_id relationship on each
  // row of our table when we drag items around, but we need to build out the
  // initial tree structure ourselves.  This means ordering our items such
  // that children items come directly after their parent items, and all items
  // are sorted by weight relative to their siblings.
  // To keep this from cluttering the actual tabledrag code, we have moved
  // this to a dedicated function.

  // Fetch the language data from the database, ordered by parent/child/weight.
  $languages = language_hierarchy_language_list();

  // Initialise checkboxes array.
  $options = array();

  // Iterate through each database result.
  foreach ($languages as $item) {

    // Create list of enabled languages and the language checkboxes options for later use.
    $options[$item->language] = '';
    if ($item->enabled) {
      $enabled[] = $item->language;
    }

    // Create a form entry for this item.
    //
    // Each entry will be an array using the the unique language code for that item as
    // the array key, and an array of table row data as the value.
    $form['language_items'][$item->language] = array(

      // We'll use a form element of type '#markup' to display the language name.
      'name' => array(
        '#markup' => $item->name,
      ),

      // Add the native name.
      'native' => array(
        '#markup' => $item->native,
      ),

      // Add the language code. We use a seperate markup code column for display as
      // the hidden one is used in the table drag.
      'code' => array(
        '#markup' => $item->language,
      ),

      // Add language direction
      'direction' => array(
        '#markup' => ($item->direction == LANGUAGE_RTL ? t('Right to left') : t('Left to right')),
      ),

      // For parent/child relationships, we also need to add form items to
      // store the current item's unique id and parent item's unique id.
      //
      // We would normally use a hidden element for this, but for this example
      // we'll use a disabled textfield element called 'id' so that we can
      // display the current item's id in the table.
      //
      // Because tabledrag modifies the #value of this element, we use
      // '#default_value' instead of '#value' when defining a hidden element.
      // Also, because tabledrag modifies '#value', we cannot use a markup
      // element, which does not support the '#value' property. (Markup
      // elements use the '#markup' property instead.)
      'id' => array(
        '#type' => 'hidden',
        '#size' => 3,
        '#default_value' => $item->language,
        '#disabled' => TRUE,
      ),

      // The same information holds true for the parent id field as for the
      // item id field, described above.
      'pid' => array(
        '#type' => 'textfield',
        '#size' => 3,
        '#default_value' => $item->parent,
      ),

      // The 'weight' field will be manipulated as we move the items around in
      // the table using the tabledrag activity.  We use the 'weight' element
      // defined in Drupal's Form API.
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $item->weight,
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),

      // We'll use a hidden form element to pass the current 'depth' of each
      // item within our parent/child tree structure to the theme function.
      // This will be used to calculate the initial amount of indentation to
      // add before displaying any child item rows.
      'depth' => array(
        '#type' => 'hidden',
        '#value' => $item->depth,
      ),
    );
  }


  // Create language enabled checkboxes items. We'll render it by row later.
  $form['enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled languages'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => $enabled,
  );

  // Site default language.
  $form['site_default'] = array(
    '#type' => 'radios',
    '#title' => t('Default language'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => language_default('language'),
  );


  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save Changes'));
  return $form;
}


/**
 * Theme callback for the language hierarchy form.
 *
 * The theme callback will format the $form data structure into a table and
 * add our tabledrag functionality.  (Note that drupal_add_tabledrag should be
 * called from the theme layer, and not from a form declaration.  This helps
 * keep template files clean and readable, and prevents tabledrag.js from
 * being added twice accidently.
 *
 */
function theme_language_hierarchy_form($variables) {
  $form = $variables['form'];

  // Initialize the variable which will store our table rows.
  $rows = array();

  // Iterate over each element in our $form['language_items'] array.
  foreach (element_children($form['language_items']) as $id) {

    // Before we add our 'weight' column to the row, we need to give the
    // element a custom class so that it can be identified in the
    // drupal_add_tabledrag call.
    //
    // This could also have been done during the form declaration by adding
    // @code
    //   '#attributes' => array('class' => 'language-item-weight'),
    // @endcode
    // directly to the 'weight' element in tabledrag_example_simple_form().
    $form['language_items'][$id]['weight']['#attributes']['class'] = array('language-item-weight');

    // In the parent/child example, we must also set this same custom class on
    // our id and parent_id columns (which could also have been done within
    // the form declaration, as above).
    $form['language_items'][$id]['id']['#attributes']['class'] = array('language-item-id');
    $form['language_items'][$id]['pid']['#attributes']['class'] = array('language-item-pid');

    // To support the tabledrag behaviour, we need to assign each row of the
    // table a class attribute of 'draggable'. This will add the 'draggable'
    // class to the <tr> element for that row when the final table is
    // rendered.
    $class = array('draggable');

    // If this is a child element, we need to add some indentation to the row,
    // so that it appears nested under its parent.  Our $depth parameter was
    // calculated while building the language data.
    $indent = theme('indentation', array('size' => $form['language_items'][$id]['depth']['#value']));
    unset($form['language_items'][$id]['depth']);

    // Disable checkbox for the default language, because it cannot be disabled.
    if ($id == language_default()->language) {
      $form['enabled'][$id]['#attributes']['disabled'] = 'disabled';
    }

    // We are now ready to add each element of our $form data to the $rows
    // array, so that they end up as individual table cells when rendered
    // in the final table.  We run each element through the drupal_render()
    // function to generate the final html markup for that element.

    // Create operations links.
    $operations_links = l(t('edit'), 'admin/config/regional/language/edit/' . $id) . (($id != 'en' && $id != language_default()->language) ? ' ' . l(t('delete'), 'admin/config/regional/language/delete/' . $id) : '');
    // Add our hidden 'id' column into the operations links column.
    $operations_links .= drupal_render($form['language_items'][$id]['id']);

    $rows[] = array(
      'data' => array(
        // Add our 'name' column, being sure to include our indentation.
        $indent . drupal_render($form['language_items'][$id]['name']),
        // Add our 'native' column.
        drupal_render($form['language_items'][$id]['native']),
        // Add 'code' column.
        drupal_render($form['language_items'][$id]['code']),
        // Add 'direction' column.
        drupal_render($form['language_items'][$id]['direction']),
        // Add 'enabled' checkboxes.
        array('data' => drupal_render($form['enabled'][$id]), 'align' => 'center'),
        // Add our 'site_default' column.
        drupal_render($form['site_default'][$id]),
        // Add operations links column with hidden IDs form elements.
        $operations_links,
        // Add our 'weight' column.
        drupal_render($form['language_items'][$id]['weight']),

        // Add our hidden 'parent id' column.
        drupal_render($form['language_items'][$id]['pid']),
      ),
      // To support the tabledrag behaviour, we need to assign each row of the
      // table a class attribute of 'draggable'. This will add the 'draggable'
      // class to the <tr> element for that row when the final table is
      // rendered.
      'class' => $class,
    );
  }

  // We now define the table header values.  Ensure that the 'header' count
  // matches the final column count for your table.
  //
  // Normally, we would hide the headers on our hidden columns, but we are
  // leaving them visible in this example.
  // $header = array(t('Name'), t('Description'), '', '', '');
  $header = array(
    t('Name'),
    t('Native name'),
    t('Code'),
    t('Direction'),
    t('Enabled'),
    t('Default'),
    t('Operations'),
    t('Weight'),
    t('PID'),
  );

  // We also need to pass the drupal_add_tabledrag() function an id which will
  // be used to identify the <table> element containing our tabledrag form.
  // Because an element's 'id' should be unique on a page, make sure the value
  // you select is NOT the same as the form ID used in your form declaration.
  $table_id = 'language-items-table';

  // We can render our tabledrag table for output.
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  // And then render any remaining form elements (such as our submit button).
  $output .= drupal_render_children($form);

  // We now call the drupal_add_tabledrag() function in order to add the
  // tabledrag.js goodness onto our page.
  //
  // For our parent/child tree table, we need to pass it:
  // - the $table_id of our <table> element (language-items-table),
  // - the $action to be performed on our form items ('match'),
  // - a string describing where $action should be applied ('parent'),
  // - the $group value (pid column) class name ('language-item-pid'),
  // - the $subgroup value (pid column) class name ('language-item-pid'),
  // - the $source value (id column) class name ('language-item-id'),
  // - an optional $hidden flag identifying if the columns should be hidden,
  // - an optional $limit parameter to control the max parenting depth.
  drupal_add_tabledrag($table_id, 'match', 'parent', 'language-item-pid', 'language-item-pid', 'language-item-id', TRUE);

  // Because we also want to sort in addition to providing parenting, we call
  // the drupal_add_tabledrag function again, instructing it to update the
  // weight field as items at the same level are re-ordered.
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'language-item-weight', NULL, NULL, TRUE);

  return $output;
}

/**
 * Submit callback for the language_hierarchy_form form.
 *
 * Updates the 'weight' column for each element in our table, taking into
 * account that item's new order after the drag and drop actions have been
 * performed.
 *
 */
function language_hierarchy_form_submit($form, &$form_state) {
  // Reset static cache language list is not fetched from database
  drupal_static_reset('language_hierarchy_language_list');
  drupal_static_reset('language_hierarchy_get_children');
  drupal_static_reset('language_hierarchy_get_root_languages');

  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  foreach ($form_state['values']['language_items'] as $langcode => $item) {

    // Update the language row in the db.
    db_update('languages')
      ->fields(array(
        'parent' => $item['pid'],
      ))
      ->condition('language', $item['id'], '=')
      ->execute();

    // Copy weights data to where locale submit function expects it.
    $form_state['values']['weight'][$item['id']] = $item['weight'];
  }

  // Run the original locale submit function to deal with core locale form functionality.
  // This includes enabling, settings default language and weight saving.
  module_load_include('inc', 'locale', 'locale.admin');
  locale_languages_overview_form_submit($form, $form_state);

  // Normalize weight, so languages are returned in correct order for flat lists.
  _language_hierarchy_normalize_weight();
}
