(function ($) {
Drupal.behaviors.tongwen = {
  attach: function (context, settings) {
    var cookie = 'tongwen';
    var cookie_options = { path: '/', expires: 100 };
    var cookie_lang = $.cookie(cookie);

    // skip if drupal language already the same
    if(settings.language == cookie_lang){
      return;
    }
    var nav_lang = (navigator.language) ? navigator.language : navigator.userLanguage;

    if(cookie_lang){
      var userLang = cookie_lang;
    }
    else{
      var userLang = nav_lang;
    }
    if((userLang.toLocaleLowerCase() == 'zh-tw' || userLang.toLocaleLowerCase() == 'zh-hk' )
     && (nav_lang.toLocaleLowerCase() !== 'zh-tw' && nav_lang.toLocaleLowerCase() !== 'zh-hk' )){
      TongWen.trans2Trad(document);
      $.cookie(cookie, 'zh-TW', cookie_options);
    }
    else if(userLang.toLocaleLowerCase() == 'zh-cn' ){
      $.cookie(cookie, 'zh-CN', cookie_options);
      TongWen.trans2Simp(document);
    }

    $('.tongwen_s2t a').click(function(){
      $.cookie(cookie, 'zh-TW', cookie_options);
      TongWen.trans2Trad(document);
    });
    $('.tongwen_t2s a').click(function(){
      $.cookie(cookie, 'zh-CN', cookie_options);
      TongWen.trans2Simp(document);
    });
  }
};
}(jQuery));
