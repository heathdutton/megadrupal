<?php
// Add tabledrag.js.
drupal_add_tabledrag('field-weight-table', 'order', 'sibling', 'field-weight'); 

// Setup static zebra to stripe table rows.
static $zebra;
$zebra = 1;
?>

<p><?php print drupal_render($form['markup']); ?></p>

<table id="field-weight-table">
  <thead>
    <th><?php print t('Fields'); ?></th>
    <th class="tabledrag-hide"><?php print t('Weight'); ?></th>
    <th><?php print t('Hide'); ?></th>
  </thead>
  <tbody>
    <?php foreach (array_keys($instances) as $key): 
      $zebra_class = $zebra % 2 ? 'odd' : 'even';
      ?>
      <tr class="draggable <?php print $zebra_class; ?>">
        <td class="field-element"><?php print drupal_render($form[$key]['field']); ?></td>
        <td class="tabledrag-hide"><?php print drupal_render($form[$key]['weight']); ?></td>
        <td class="field-hidden"><?php print drupal_render($form[$key]['hidden']); ?></td>
      </tr>
      <?php $zebra++; ?>
    <?php endforeach; ?>
  </tbody>
</table>
