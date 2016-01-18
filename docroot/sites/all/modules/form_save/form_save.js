(function($) {

  Drupal.form_save = Drupal.form_save || {};
  Drupal.form_save.currentForm = null;
  Drupal.form_save.controlDown = false;
  Drupal.form_save.clickBubble = false;

  /**
   * Collapse modules on Permissions page.
   */
  Drupal.behaviors.formSave = {
    attach: function (context, settings) {
      if (settings.form_save.enabled) {
        // we need to manually keep track of which form is focuses as the focus
        // event only works on form elements. We want it to work on divs and any
        // other element inside the form.
        // We also have to keep track of the click bubble as we can return false
        // or this would stop form elements for working correctly.
        // We add the focus in event handler to grab the focus when tabbing through elements.
        $('form').focusin(function(){
          Drupal.form_save.currentForm = this;
        }).click(function(){
          Drupal.form_save.currentForm = this;
          Drupal.form_save.clickBubble = true;
        });
        $(document).click(function(){
          if(Drupal.form_save.clickBubble) {
            Drupal.form_save.clickBubble = false;
          }
          else{
            Drupal.form_save.currentForm = null;
          }
        });

        // fix enter submission of form via textfield to make sure it clicks default button
        $('form input[type=text]').keydown(function(event) {
          if (event.keyCode == 13) {
            $(this).parents('form').find('input.form-save-default-button').click();
          }
        });

        $(document).keydown(function(event){
          // if Ctrl or Cmd
          if(event.which == 17 || event.which == 91){
            Drupal.form_save.controlDown = true;
          }

          // if key is s and ctrl or cmd is currently in down state then
          // submit form with click of default button.
          if(Drupal.form_save.currentForm && Drupal.form_save.controlDown && event.which == 83) {
            $('input.form-save-default-button', Drupal.form_save.currentForm).click();
            return false;
          }
        }).keyup(function(event){
          // remove ctrl or cmd
          if(event.which == 17 || event.which == 91){
            Drupal.form_save.controlDown = false;
          }
        });
      }
    }
  };

})(jQuery);
