/**
 * @file affiliates.js
 *
 */

/**
 * Controls Affiliates UI during the creation of new banners.
 */
Drupal.behaviors.affiliates = function() {
  $('select#edit-type').change(function() {	
  if ($("select#edit-type").val() == 'text') { 
    $("#edit-image").attr('disabled', true); 
    $("#edit-anchor").attr('disabled', true); 
  }
  else { 
    $("#edit-image").removeAttr("disabled");
    $("#edit-anchor").removeAttr("disabled"); 
  } 
  });

  if ($("select#edit-type").val() == 'text') { 
    $("#edit-image").attr('disabled', true); 
    $("#edit-anchor").attr('disabled', true); 
  }
  else { 
    $("#edit-image").removeAttr("disabled");
    $("#edit-anchor").removeAttr("disabled"); 
  }  
}