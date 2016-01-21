/**
 * @file
 * commerce_pdas.js
 */

(function ($, Drupal, window, document, undefined) {

  Drupal.behaviors.commercePDAS = {

    /**
     * Page Load Behavior
     */
    attach: function(context, settings) {
      Drupal.behaviors.commercePDAS.updateAttributes();
    },

    /**
     * Update each attribute field based on any querystring data.
     *
     * Looking at each potential attribute field, check to see if querystring
     * values exist. If so then lookup that 'name' in the settings and grab the
     * options ID (term ID) and update the field selection.
     *
     * After each attribute field is modified (and change triggered) the form
     * gets ajax update. This causes this function to be called multiple times.
     * We need to be able to track which fields we've already updated and which
     * fields still need to be updated. The -processed classes help track this.
     *
     * @todo is there a better way to handle this with the add to cart ajax
     * issue?
     */
    updateAttributes: function() {
      $.each(Drupal.settings.commercePDAS, function(shortName, attributeInfo) {
        attributeSelector = '.' + attributeInfo.attributeSelector;
        attributeOptions = attributeInfo.attributeOptions;

        attributeProcessedClass = attributeInfo.attributeSelector + '-processed';
        processedClass = 'pdas-processed';

        // If we haven't processed any attribute fields yet...
        if (!$('body').hasClass(processedClass) && !$('body').hasClass(attributeProcessedClass)) {
          // Convert to lowercase to allow for case insensitive querystring values.
          querystringValue = Drupal.behaviors.commercePDAS.getParameterByName(shortName).toLowerCase();

          // Make sure we have a value in the querystring and it's a valid
          // attribute option.
          if (querystringValue.length && attributeOptions.hasOwnProperty(querystringValue)) {
            currentValue = $(attributeSelector).val();
            // Only make a change if the current value is different than the
            // querystring value. This limits the ajax trigger.
            if (currentValue !== undefined && currentValue != attributeOptions[querystringValue]) {
              optionSelector = attributeSelector + ' option[value="' + currentValue + '"]';

              // Make sure the option exists before attempting to chagne to it.
              if ($(optionSelector).length > 0) {
                $(attributeSelector).val(attributeOptions[querystringValue]).trigger('change');
                $('body').addClass(attributeProcessedClass).addClass(processedClass);
              }
            }
          }
        }
        else {
          $('body').removeClass(processedClass);
        }
      });
    },

    /**
     * Get querystring value based on a name.
     *
     * @see http://stackoverflow.com/questions/901115/how-can-i-get-query-string-values-in-javascript
     */
    getParameterByName: function(name) {
      name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
      var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
      results = regex.exec(location.search);
      return results === null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
    }

  };

})(jQuery, Drupal, this, this.document);
