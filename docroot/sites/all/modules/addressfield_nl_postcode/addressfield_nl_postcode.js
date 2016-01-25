(function ($) {

  $.fn.NlPostcode_injectValues = function(data) {
    if (typeof data != 'undefined' && typeof data.street != 'undefined') {
      $(this).data(data);
    }
    if (typeof $(this).data() == 'undefined' ||
        typeof $(this).data().street == 'undefined' ||
        typeof $(this).data().city == 'undefined') {
      return;
    }
    var base_id = $(this).attr('id');
    var city = $(this).data().city;
    var addressline = $(this).data().street.replace(/(^\s*)|(\s*$)/g, '');

    var addition_options = {};
    var number_of_options = 1;
    addition_options['-99999'] = Drupal.t('None');
    for(x in $(this).data().houseNumberAdditions) {
      var option = $(this).data().houseNumberAdditions[x];
      if (option.length) {
        addition_options[option] = option;
        number_of_options++;
      }
    }

    var $addition_elem = $(this).find('#' + base_id + '-huisnummer-addition').first();
    if (number_of_options > 1) {
      var tmp_val = $addition_elem.val();
      if ($addition_elem.is('input')) {
        var attr_to_copy = ['id', 'name', 'class'];
        $addition_elem.replaceWith('<select id="' + $addition_elem.attr('id') + '" name="' + $addition_elem.attr('name') + '" class="' + $addition_elem.attr('class') + '"></select>');
        $addition_elem = $(this).find('#' + base_id + '-huisnummer-addition').first();
        $addition_elem.removeClass('form-text').addClass('form-select');
        $addition_elem.closest('.form-item').removeClass('form-type-textfield').addClass('form-type-select');
        Drupal.attachBehaviors($addition_elem.closest('.form-item'));
      }
      $('option', $addition_elem).remove();
      for(opt in addition_options) {
        $addition_elem.append($('<option></option>').attr('value', opt).text(addition_options[opt]));
      }
      $addition_elem.val(tmp_val);
      $addition_elem.closest('.form-item').show();
    }
    else {
      $addition_elem.val('');
      $addition_elem.closest('.form-item').hide();
    }

    // street can be empty.

    $(this).find('#' + base_id + '-postal-code').removeAttr('disabled');
    $(this).find('#' + base_id + '-huisnummer').removeAttr('disabled');
    if (addressline.length) {
      var huisnummer = $(this).find('#' + base_id + '-huisnummer').val().replace(/(^\s*)|(\s*$)/g, '');
      var huisnummer_addition = $(this).find('#' + base_id + '-huisnummer-addition').val().replace(/(^\s*)|(\s*$)/g, '');
      if (huisnummer_addition == '-99999') {
        huisnummer_addition = '';
      }
      var addressline = (addressline + ' ' + huisnummer).replace(/(^\s*)|(\s*$)/g, '');
      addressline = (addressline + ' ' + huisnummer_addition).replace(/(^\s*)|(\s*$)/g, '');
    }

    // set values.
    var $address_wrapper = $(this).closest('fieldset');
    if (!$address_wrapper.length) {
      // Not in a fieldset, some hip designer has f***** up the theme.
      // Let us hope he or she left in a class.
      $address_wrapper = $(this).closest('.field-type-addressfield');
    }
    if ($address_wrapper.length) {
      $address_wrapper.find('input.thoroughfare').val(addressline);
      $address_wrapper.find('input.locality').val(city);
    }
  };

  Drupal.NlPostcode = Drupal.NlPostcode || {};

  Drupal.NlPostcode.normalizePostcodeInput = function(elem) {
    $(elem).removeClass('error');
    var val = $(elem).val();
    val = val.replace(/(^\s*)|(\s*$)/g, ''); // trim
    if (val.match(/^[1-9][0-9]{3}[\s]*[A-Z]{2}$/i)) {
      var num = val.substring(0, 4);
      var let = val.substring(4).replace(/^\s*/g, '').toUpperCase();
      val = num + ' ' + let;
      $(elem).val(val);
      return val;
    } else {
      if (val.length) {
        $(elem).addClass('error');
      }
      return false;
    }
  };

  Drupal.NlPostcode.normalizeHuisnummerInput = function(elem) {
    $(elem).removeClass('error');
    var val = $(elem).val();
    val = val.replace(/(^\s*)|(\s*$)/g, ''); // trim
    if (val.match(/^[1-9][0-9]*$/i)) {
      $(elem).val(val);
      return val;
    } else {
      if (val.length) {
        $(elem).addClass('error');
      }
      return false;
    }
  };

  Drupal.NlPostcode.checkValues = function(e) {
    $elem = $(e.target);
    $parent = $elem.closest('.addressfield-nl-postcode');
    var base_id = $parent.attr('id');
    $elem_postcode = $parent.find('#' + base_id + '-postal-code');
    $elem_huisnummer = $parent.find('#' + base_id + '-huisnummer');
    var postcode = Drupal.NlPostcode.normalizePostcodeInput($elem_postcode);
    var huisnummer = Drupal.NlPostcode.normalizeHuisnummerInput($elem_huisnummer);
    if (postcode && huisnummer) {
      $parent.trigger('postcoderefresh');
    }
  };

  Drupal.behaviors.NlPostcode = {
    attach: function(context, settings) {
      // Hide when no options.
      $('.huisnummer-addition-nl-postcode', context).each(function() {
        if ($('option', this).length > 1) {
          $(this).closest('.form-item').show();
        } else {
          $(this).closest('.form-item').hide();
        }
      });
      $('.postal-code-nl-postcode', context).bind('change', Drupal.NlPostcode.checkValues);
      $('.huisnummer-nl-postcode', context).bind('change', Drupal.NlPostcode.checkValues);
      $('.huisnummer-addition-nl-postcode', context).bind('change', function(e){
        $parent = $(e.target).closest('.addressfield-nl-postcode');
        $parent.NlPostcode_injectValues();
      });

      // Attach ajax.
      $('.addressfield-nl-postcode', context).each(function(){
        $(this).unbind('postcoderefresh');
        var html_id = $(this).attr('id');
        var ajax_call_settings = {};
        ajax_call_settings.url = settings.basePath + 'addressfield-nl-postcode';
        ajax_call_settings.event = 'postcoderefresh';
        ajax_call_settings.keypress = false;
        ajax_call_settings.prevent = false;
        ajax_call_settings.progress = {
          type: 'throbber',
          message: Drupal.t('Please wait...')
        };
        var ajax = new Drupal.ajax(html_id, $(this), ajax_call_settings);
        ajax.orgBeforeSerialize = ajax.beforeSerialize;
        ajax.beforeSerialize = function (element, options) {
          this.orgBeforeSerialize(element, options);
          options.data['base_id'] = $(element).attr('id');
          options.data['postcode'] = $(element).find('input.postal-code-nl-postcode').attr('disabled', true).val();
          options.data['huisnummer'] = $(element).find('input.huisnummer-nl-postcode').attr('disabled', true).val();
        };
        Drupal.ajax[html_id] = ajax;
      });
      // Trigger it once.
      $('.postal-code-nl-postcode', context).trigger('change');
    }
  };

})(jQuery);