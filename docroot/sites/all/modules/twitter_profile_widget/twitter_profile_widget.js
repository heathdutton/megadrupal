(function ($) {

Drupal.behaviors.twitterProfileWidget = {
  attach: function (context) {
    //set options for colorpicker
    //more info about options you can find at http://www.eyecon.ro/colorpicker/#implement
    var options = {
      onSubmit: function(hsb, hex, rgb, el) {
        $(el).val(hex);
        $(el).ColorPickerHide();
      },
      onBeforeShow: function () {
        $(this).ColorPickerSetColor(this.value);
      }
    };

    //enable colorpicker for inputs with class "colorselect"
    $('input.colorselect')
      .ColorPicker(options)
      .bind('keyup', function(){
        $(this).ColorPickerSetColor(this.value);
      });
    
    //hide some fields on widget type select
    var list_settings  = $('.twitter-list-setting');
    var faves_settings = $('.twitter-faves-setting');
    $('select[name=twitter_profile_widget_type]').change(function() {
      var type = $(this).val();
      var speed = '100';
      if (type == 'profile') {
        list_settings.slideUp(speed);
        faves_settings.slideUp(speed);
      }
      else if (type == 'faves') {
        list_settings.slideUp(speed);
        faves_settings.slideDown(speed);
      }
      else if (type == 'list') {
        list_settings.slideDown(speed);
        faves_settings.slideUp(speed);
      }
    });

    //hide color field on shell bg type select
    var shell_bg = $('input[name=twitter_profile_widget_shell_bg_color]').parent().parent();
    $('select[name=twitter_profile_widget_shell_bg_color_type]').change(function (){
      var type = $(this).val();
      var speed = '100';
      if (type == 'color') {
        shell_bg.slideDown(speed);
      }
      else if (type == 'transparent') {
        shell_bg.slideUp(speed);
      }
    });

    //hide color field on shell bg type select
    var tweet_bg = $('input[name=twitter_profile_widget_tweet_bg_color]').parent().parent();
    $('select[name=twitter_profile_widget_tweet_bg_color_type]').change(function (){
      var type = $(this).val();
      var speed = '100';
      if (type == 'color') {
        tweet_bg.slideDown(speed);
      }
      else if (type == 'transparent') {
        tweet_bg.slideUp(speed);
      }
    });

  }
};

})(jQuery);