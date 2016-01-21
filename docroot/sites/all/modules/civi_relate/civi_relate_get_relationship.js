(function ($) {
  Drupal.behaviors.civi_relate = {
    attach: function (context, settings) {
      var formDialog = $('<div></div>').load(Drupal.settings.basePath + 'civi-relate/choose #civi-relate-relationship-choose-form')
        .dialog({
          title: 'Select a Relationship',
          modal: true
        });  
  
    $("body").ajaxComplete(function(){
      var options = {
        url :     Drupal.settings.basePath + 'system/ajax',
        dataType: 'json',
        success:  function() {
         $('#crm-container').prepend('<div>Relationship Selection Recorded</div>');
          formDialog.dialog( "close" );
        }
      };
      $('#civi-relate-relationship-choose-form').ajaxForm(options);
      $('input[name=civi_relate_dynamic_source]').val('js');
    });
    }
  };
}(jQuery));

