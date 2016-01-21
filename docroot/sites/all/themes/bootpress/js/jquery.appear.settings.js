/**
 * Theme jQuery Appear Settings
 * @author Pitabas Behera
*/
jQuery(function() {

  jQuery('.animated').appear();

  jQuery(document.body).on('appear', '.fade', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fade')
    });
  });
  jQuery(document.body).on('appear', '.slide-animate', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-slide-animate')
    });
  });
  jQuery(document.body).on('appear', '.hatch', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-hatch')
    });
  });
  jQuery(document.body).on('appear', '.entrance', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-entrance')
    });
  });
  jQuery(document.body).on('appear', '.tada', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-tada')
    });
  });
  jQuery(document.body).on('appear', '.rotate-up', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-rotate-up')
    });
  });
  jQuery(document.body).on('appear', '.rotate-down', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-rotate-down')
    });
  });
  jQuery(document.body).on('appear', '.fadeInDown', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInDown')
    });
  });
  jQuery(document.body).on('appear', '.fadeInUp', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInUp')
    });
  });
  jQuery(document.body).on('appear', '.fadeInLeft', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInLeft')
    });
  });
  jQuery(document.body).on('appear', '.fadeInRight', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInRight')
    });
  });
  jQuery(document.body).on('appear', '.fadeInDownBig', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInDownBig')
    });
  });
  jQuery(document.body).on('appear', '.fadeInUpBig', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInUpBig')
    });
  });
  jQuery(document.body).on('appear', '.fadeInLeftBig', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInLeftBig')
    });
  });
  jQuery(document.body).on('appear', '.fadeInRightBig', function () {
    jQuery(this).each(function () {
      jQuery(this).addClass('ae-animation-fadeInRightBig')
    });
  });


});