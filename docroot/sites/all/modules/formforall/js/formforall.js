/**
 * @file
 * Process formforall elements.
 */

(function ($, Drupal) {

  /* *********************************************************************
   * INIT
   * ********************************************************************/

  // FormForAll global object
  Drupal.formForAll = Drupal.formForAll || {};

  // Store FormForAll forms in an Array
  Drupal.formForAll.FFAforms = Drupal.formForAll.FFAforms || [];

  // FormForAll JS Settings
  Drupal.settings.formForAll = Drupal.settings.formForAll || {};


  /* *********************************************************************
   * FUNCTIONS
   * ********************************************************************/

  Drupal.formForAll.initForms = function(element) {
    Drupal.formForAll.FFAforms.push(new __FFA(
      element,
      element.attr("id").replace("formforall-", ""),
      Drupal.settings.formForAll.domain
    ))
  }


  /* *********************************************************************
   * BEHAVIORS
   * ********************************************************************/

  Drupal.behaviors.formForAll = {
    attach: function(context, settings) {
      if ($('.formforall-form:not(.formforall-processed)', context).length) {
        // FFA lib is not loaded yet.
        if (typeof(__FFA) == 'undefined') {
          // Load script.
          $.ajax({
            url: 'https://' + Drupal.settings.formForAll.domain + '/assets/javascripts/ffa.js',
            dataType: "script",
            success: function(){
              // Init FFA forms.
              $('.formforall-form', context).once('formforall', function(){
                Drupal.formForAll.initForms($(this));
              });
            }
          });
        }
        else {
          // Init FFA forms.
          $('.formforall-form', context).once('formforall', function(){
            Drupal.formForAll.initForms($(this));
          });
        }
      }
    }
  }

})(jQuery, Drupal);
