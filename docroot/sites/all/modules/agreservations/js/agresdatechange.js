(function ($) {
  Drupal.behaviors.agreservations = {
    attach: function(context, settings) {
      $('#edit-checkin-datepicker-popup-0',context).change(function () {
        var strdate = $(this).attr("value");
        var msecsdate  = Date.parse(strdate);
        var interval = +1;
        var dateto = new Date(msecsdate);
        dateto.setDate(dateto.getDate() -0+interval);        
        var tdate = document.getElementById("edit-checkout-datepicker-popup-0");
        var result = dateto.getFullYear()+'-'+(dateto.getMonth()+1)+'-'+dateto.getDate(); //dateto.toLocaleFormat("%Y-%m-%d");  
        tdate.value = result
      });
    }
  }
})(jQuery);