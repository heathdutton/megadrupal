(function($){
  Drupal.behaviors.resource_booking = {
    attach:function (context) {
        if (typeof Drupal.settings.resource_booking != 'undefined') {
          var datefield = Drupal.settings.resource_booking['date_field'];
          var onevent = 'change';
          $('[name="' + datefield + '[und][0][value][date]"]').bind(onevent, setValue);
          $('[name="' + datefield + '[und][0][value][time]"]').bind(onevent, setValue);
          $('[name="' + datefield + '[und][0][value2][date]"]').bind(onevent, setValue);
          $('[name="' + datefield + '[und][0][value2][time]"]').bind(onevent, setValue);
          //fire change event to ensure on load the date & times are synced
          $('[name="' + datefield + '[und][0][value][date]"]').change();
          $('[name="' + datefield + '[und][0][value][time]"]').change();
          $('[name="' + datefield + '[und][0][value2][date]"]').change();
          $('[name="' + datefield + '[und][0][value2][time]"]').change();
        }
    }
  }
  var setValue = function() {
    var fname = $(this).attr('name');
    var datefield = Drupal.settings.resource_booking['date_field'];
    var rbfield = Drupal.settings.resource_booking['rb_field'];
    var name = '';
    if ((fname.indexOf("value2") > 0)) {
      name = rbfield + '[und][edate]';
    }
    else {
      name = rbfield + '[und][sdate]';
    }
    if ((fname.indexOf("time") > 0)) {
      name += '[time]';
    }
    else {
      name += '[date]';
    }
    $('[name="' + name + '"]').val($(this).val());
    if (name.indexOf('[date]') > 0){
      //set time value as well
      var tname = '[name="' + fname.replace('[date]', '[time]')  + '"]';
      var tfield = name.replace('[date]', '[time]');
      $('[name="' + tfield + '"]').val($(tname).val());
    }
  }
})(jQuery);
