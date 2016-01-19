
/**
 * JavaScript behaviors for the front-end display of webforms.
 */

(function ($) {

  Drupal.webform = Drupal.webform || {};

  /**
   * Utility function to get an array of [product id, quantity] values
   * from a product field.
   *
   * @return Array[Array[int, int]]
   *   Returns an array of arrays. Each sub array has to elements:
   *   0: A product id
   *   1: The quantity of that product id
   */
  Drupal.webform.productfieldValue = function(element, existingValue) {
    var value = [];
    var quantity = parseInt($(element).find('.productfield-quantity').val(), 10);
    quantity = isNaN(quantity, 10) ? 1 : quantity;

    if (element) {
      // Checkboxes and radio product choices.
      $(element).find('input[type=checkbox]:checked,input[type=radio]:checked').each(function() {
        var checkboxValue = parseInt(this.value, 10);
        checkboxValue = isNaN(checkboxValue) ? 0 : checkboxValue;
        value.push([checkboxValue, quantity]);
      });

      // Lets see if its multiple selection with drop down quanities.
      if (!value.length) {
        $(element).find('select.productfield-quantity').each(function() {
          var idParts = this.id.split('-');
          var productId = parseInt(idParts[6], 10);
          if (!isNaN(productId) && productId > 0) {
            value.push([productId, parseInt(this.value, 10)]);
          }
        });
      }

      // Single selection with from select list of products.
      if (!value.length) {
        $(element).find('select').each(function() {
          var selectValue = parseInt(this.value, 10);
          selectValue = isNaN(selectValue) ? 0 : selectValue;
          if (selectValue instanceof Array) {
            $.each(selectValue, function(n, selectSubvalue) {
              value.push([selectSubvalue, quantity]);
            });
          }
          else {
            value.push([selectValue, quantity]);
          }
        });
      }

      // If not checkboxes or a select list it might be multiple quantity text fields.
      if (!value.length) {
        $(element).find('input[type=text]').each(function() {
          var idParts = this.id.split('-');
          var productId = parseInt(idParts[6], 10);
          if (!isNaN(productId) && productId > 0) {
            value.push([productId, parseInt(this.value, 10)]);
          }
        });
      }

    }
    else if (existingValue) {
      value = existingValue;
    }

    return value;
  };

  /**
   * Determine if a product field element has one of a list of products selected.
   */
  Drupal.webform.productfieldHasSelected = function(productfieldElement, existingValue, products) {
    var found = false;
    var currentValue = Drupal.webform.productfieldValue(productfieldElement, existingValue);

    $.each(currentValue, function(n, value) {
      $.each(products, function(k, productId) {
        if (value[0] === productId && value[1] > 0) {
          found = true;
          return false;
        }
      });

      if (found) {
        return false;
      }
    });

    return found;
  };

  Drupal.webform.conditionalOperatorProductEqual = function(element, existingValue, ruleValue) {
    var productId = parseInt(ruleValue, 10);
    return Drupal.webform.productfieldHasSelected(element, existingValue, [productId]);
  };

  Drupal.webform.conditionalOperatorProductNotEqual = function(element, existingValue, ruleValue) {
    var productId = parseInt(ruleValue, 10);
    return !Drupal.webform.productfieldHasSelected(element, existingValue, [productId]);
  };

  Drupal.webform.conditionalOperatorProductOfType = function(element, existingValue, ruleValue) {
    return Drupal.webform.productfieldHasSelected(element, existingValue, ruleValue);
  };

  Drupal.webform.conditionalOperatorProductNotOfType = function(element, existingValue, ruleValue) {
    return !Drupal.webform.productfieldHasSelected(element, existingValue, ruleValue);
  };

  Drupal.webform.conditionalOperatorProductQuantityEquals = function(element, existingValue, ruleValue) {
    var currentValue = Drupal.webform.productfieldValue(element, existingValue);
    var total = 0;

    $.each(currentValue, function(n, value) {
      total += value[1];
    });

    return total == parseInt(ruleValue, 10);
  };

  Drupal.webform.conditionalOperatorProductQuantityLessThan = function(element, existingValue, ruleValue) {
    var currentValue = Drupal.webform.productfieldValue(element, existingValue);
    var total = 0;

    $.each(currentValue, function(n, value) {
      total += value[1];
    });

    return total < parseInt(ruleValue, 10);
  };

  Drupal.webform.conditionalOperatorProductQuantityGreaterThan = function(element, existingValue, ruleValue) {
    var currentValue = Drupal.webform.productfieldValue(element, existingValue);
    var total = 0;

    $.each(currentValue, function(n, value) {
      total += value[1];
    });

    return total > parseInt(ruleValue, 10);
  };

})(jQuery);
