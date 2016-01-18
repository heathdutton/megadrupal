(function ($) {
  Drupal.behaviors.jqueryui_theme_settings = {
    attach: function() {
      
      $('.submit-jqueryui-settings').hide();
      
      $(document).change(function() {
        if ($('.box-row').is(':checked')) {
          $('.submit-jqueryui-settings').show();
        }
        else {
          $('.submit-jqueryui-settings').hide();
        }
      });
     
      $('.box-all').change(function(){
        if ($(this).is(':checked')) {
          $('.box-row').attr('checked', true);
        }
        else {
          $('.box-row').attr('checked', false);
        }
      });
      
      $(".box-drupal-theme, .textfield-name").change(function(){
        var ele_class = $(this).attr('class');
        var theme = getTheme('jqueryui', ele_class);
        $('.box-row-' + theme).attr('checked', true);
      });
      
      $(".box-drupal-theme").change(function(){
        if ($(this).is(':checked')) {
          var box_class = $(this).attr('class');
          var drupal_theme = getTheme('drupal', box_class);
          
          var elem = $('.box-cell-' + drupal_theme).not(this);
          $.each(elem, function(){
            if (this.checked) {
              var theme = getTheme('jqueryui', this.className);
              this.checked = false;
              $('.box-row-' + theme).attr('checked', true);
            }
          });
        }
      });
    } 
  };
  
  function getTheme(type, ele_class) {
    var ele_class = ele_class.split(' ');
    var i = 0, theme = '', reg = '';
    if (type == 'jqueryui') {
      reg = /^(box|textfield)-jqueryui-[a-z_]*/;
    }
    else if (type == 'drupal') {
      reg = /^box-cell-[a-z0-9_]*/;
    }
    
    while(i < ele_class.length && theme == '') {
      if (ele_class[i].match(reg)) {
        theme = ele_class[i].split('-');
        theme = theme[2];
      }
      i++;
    }
    return theme;
  }
})(jQuery);