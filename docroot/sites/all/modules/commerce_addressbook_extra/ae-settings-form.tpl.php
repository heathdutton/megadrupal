
<table class="addressbook-extra-table">  
  <thead>
    <tr>
      <th><strong>Customer profile</strong></th>          
      <th align="center"><strong>Add to Register form</strong></th>
      <th align="center"><strong>Show on Profile edit form</strong></th>
      <th align="center"><strong>Enable Address Book</strong></th>
      <th align="center"><strong>Default field</strong></th>
    </tr> 
  </thead>
  <tbody>
    <?php foreach ($customer_profiles as $type => $name) : ?>       
    <tr>
      <td><label for="edit-customer-profiles-<?php print str_replace('_', '-', $type); ?>"><?php print $name; ?></label></td>
      <td class="register-cell"><?php print drupal_render($form['customer_profiles'][$type]); ?></td>
      <td class="profile-edit-cell"><?php print drupal_render($form[$show_profile_fn[$type]]); ?></td>
      <td class="addr-enable-cell"><?php print drupal_render($form[$addr_book_enable_fn[$type]]); ?></td>      
      <td align="center"><?php print drupal_render($form[$default_field_fn[$type]]); ?></td>     
    </tr>    
    <?php endforeach ?>
  </tbody>
</table>

<?php print drupal_render_children($form); ?>
