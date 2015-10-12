// $Id: matrix.js,v 1.1.2.9.2.1 2011/01/18 10:10:05 aaron1234nz Exp $

(function($) {
  //Global container.
  window.matrix = {

    /**
     * window.matrix.sum()
     * @param elements
     *     Class of elements to act on
     * @param field_name
     *     The name of the field
     * @param destination
     *     The ID of the div to place the result
     * @param prefix
     *    String to put in front of the result
     * @param suffix
     *    String to put afer the result
     * @param rounding
     *     Number of decimal places to round to
     */
    sum: function(elements, destination, field_name, prefix, suffix, rounding) {
      var sum = 0;
      $('#matrix-field-'+ field_name +' .' + elements).each(function(index){
        var num = parseFloat($(this).val());
        if (!isNaN(num)) {
          sum += num;
        }
      });
      if (!isNaN(rounding)) {
        sum = sum.toFixed(rounding);
      }
      $('#matrix-result-'+ field_name +'-'+ destination).html(prefix + sum + suffix);
      $('#matrix-field-'+ field_name +' .matrix-cell-' + destination).val(prefix + sum + suffix);
    },

    /**
     * window.matrix.average()
     * @param elements
     *     Class of elements to act on
     * @param destination
     *     The ID of the div to place the result
     * @param field_name
     *     The name of the field
     * @param prefix
     *    String to put in front of the result
     * @param suffix
     *    String to put afer the result
     * @param rounding
     *     Number of decimal places to round to
     */
    average: function(elements, destination, field_name, prefix, suffix, rounding) {
      var total = 0;
      var average = 0;
      var count = $('.' + elements).length;
      $('#matrix-field-'+ field_name +' .' + elements).each(function(index){
        var num = parseFloat($(this).val());
        if (!isNaN(num)) {
          total += num;
        }
      });
      if (count > 0) {
        average = total/count;
      }
      if (!isNaN(rounding)) {
        average = average.toFixed(rounding);
      }
      $('#matrix-result-'+ field_name +'-'+ destination).html(prefix + average + suffix);
      $('#matrix-field-'+ field_name +' .matrix-cell-' + destination).val(prefix + average + suffix);
    },

    /**
     * window.matrix.min()
     * @param elements
     *     Class of elements to act on
     * @param destination
     *     The ID of the div to place the result
     * @param field_name
     *     The name of the field
     * @param prefix
     *    String to put in front of the result
     * @param suffix
     *    String to put afer the result
     * @param rounding
     *     Number of decimal places to round to
     */
    min: function(elements, destination, field_name, prefix, suffix, rounding) {
      var min;
      $('#matrix-field-'+ field_name +' .' + elements).each(function(index){
        var num = parseFloat($(this).val());
        if (isNaN(min) && !isNaN(num)) {
          min = num;
        }
        if (!isNaN(num) && num < min) {
          min = num;
        }
      });
      if (!isNaN(rounding)) {
        min = min.toFixed(rounding);
      }
      $('#matrix-result-'+ field_name +'-'+ destination).html(prefix + min + suffix);
      $('#matrix-field-'+ field_name +' .matrix-cell-' + destination).val(prefix + min + suffix);
    },

    /**
     * window.matrix.max()
     * @param elements
     *     Class of elements to act on
     * @param destination
     *     The ID of the div to place the result
     * @param field_name
     *     The name of the field
     * @param prefix
     *    String to put in front of the result
     * @param suffix
     *    String to put afer the result
     * @param rounding
     *     Number of decimal places to round to
     */
    max: function(elements, destination, field_name, prefix, suffix, rounding) {
      var max;
      $('#matrix-field-'+ field_name +' .' + elements).each(function(index){
        var num = parseFloat($(this).val());
        if (isNaN(max) && !isNaN(num)) {
          max = num;
        }
        if (!isNaN(num) && num > max) {
          max = num;
        }
      });
      if (!isNaN(rounding)) {
        max = max.toFixed(rounding);
      }
      $('#matrix-result-'+ field_name +'-'+ destination).html(prefix + max + suffix);
      $('#matrix-field-'+ field_name +' .matrix-cell-' + destination).val(prefix + max + suffix);
    },

    /**
     * window.matrix.custom()
     * @param callback
     *     Name of the PHP callback function
     * @param elements
     *     Class of elements to act on
     * @param destination
     *     The ID of the div to place the result
     * @param field_name
     *     The name of the field
     */
    custom: function(callback, elements, destination, field_name) {
      var data = [];

      $('#matrix-field-'+ field_name +' .' + elements).each(function(index){
        data.push($(this).val());
      });

      $.ajax({
         type: "POST",
         url: Drupal.settings.basePath + "matrix/calculation",
         data: "callback=" + callback + "&data=" + data,
         success: function(result) {
          if (result['error']) {
            alert(result['error']);
          }
          else {
             $('#matrix-result-'+ field_name +'-'+ destination).html(result['data']);
             $('#matrix-field-'+ field_name +' .matrix-cell-' + destination).val(result['data']);
          }
        }
      });
    }
  }
})(jQuery);