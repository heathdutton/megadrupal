<?php
/**
 * @file
 * Customize the display of a complete Quick Field form.
 */
?>


<table>
  <tr id = "-add-new-field" class = "add-new odd">
    <td>    
      <?php print drupal_render($form['label']); ?>
    </td>
    <td class = "tabledrag-hide" style = "display: none;"><div class = "add-new-placeholder">&nbsp;
      </div>
      <?php print drupal_render($form['weight']); ?>
    </td>
    
    <td><div class = "add-new-placeholder">&nbsp;
      </div>
      <?php print drupal_render($form['field_name']); ?>
      <small id = "edit-fields-add-new-field-label-machine-name-suffix" style = "display: none;"> <span class = "machine-name-value"></span> <span class = "admin-link"><a href = "#">Edit</a></span></small></td><td><div class = "add-new-placeholder">&nbsp;
      </div>
      <?php print drupal_render($form['type']); ?>
    </td>
    <td colspan = "3"><div class = "add-new-placeholder">&nbsp;
      </div>
     <?php print drupal_render($form['widget_type']); ?>
    </td> 
  </tr>
</table>

<?php
  // Print out the main part of the form.
  print drupal_render_children($form);
?>
