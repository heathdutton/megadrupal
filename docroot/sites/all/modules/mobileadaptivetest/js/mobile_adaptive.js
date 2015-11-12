(function ($) {
  $(document).ready(function() {
    // Set constants
    var blockPositionLeft = 15;
    // Set page path
    var path = window.location.href;
    var script_path = Drupal.settings.basePath + Drupal.settings.mobile_adaptive_script_path;
    // Check if script should be initialized
    if (!isInitAllowed()) {
      return;
    }
    // Init Device Array
    devices = DevicesList();
    // Init mobile adaptive script
    var mobileAdaptive = new MobileAdaptive(script_path, path);
    mobileAdaptive.init();
    if (localStorage['mobile_adaptive_device']) {
      var default_mode = localStorage['mobile_adaptive_mode'];
      var default_os = localStorage['mobile_adaptive_os'];
      var default_device = localStorage['mobile_adaptive_device'];
      UpdateModeSelect(default_mode);
      if (devices[default_mode]['#no-os']) {
        $('#os').hide();
      }
      else {
        $('#os').val(default_os);
        UpdateDeviceList(default_os);
      }
      $('#device').val(default_device);
      UpdateDevice();
    }
    else {
      mode = $('#mode-select div:first').attr('id');
      UpdateModeSelect(mode);
    }
    hideDevice();
    // Set event handlers
    $('#mobile-adaptive-switch').click(function() {
      if (!$(this).hasClass('active')) {
        $(this).addClass('active');
        var device = $('#mobile-adaptive-block #configs.active #device').val();
        showDevice(device);
        $('#mobile-adaptive-block').animate({'left': blockPositionLeft});
      }
      else {
        $(this).removeClass('active');
        hideDevice();
        $('#mobile-adaptive-block').animate({'left': -210});
      }
    });
    
    $('#mobile-adaptive-block #mode-select div').click(function() {
      UpdateModeSelect(this['id']);
    });

    $('#mobile-adaptive-block #os').change(function() {
       UpdateDeviceList($('#os').val());
    });
    
    $('#mobile-adaptive-block select#device').change(function() {
      UpdateDevice();
    });
    
    $('.rotate').click(function() {
      $('.rotate#landscape').show();
      $('.rotate#portrait').show();
      $(this).hide();
      localStorage['mobile_adaptive_rotate'] = $('.rotate:visible').attr('id');
      mobileAdaptive.rotate();
    });
    
    function UpdateDevice() {
      var device = $('#mobile-adaptive-block #configs.active #device').val();
      showDevice(device);
    }

    function showDevice(device) {
      mobileAdaptive.show(device);
      var rotate = (localStorage['mobile_adaptive_rotate']) ? localStorage['mobile_adaptive_rotate'] : 'portrait';
      $('.rotate').hide();
      $('.rotate#' + rotate).show();
      if (rotate != 'portrait' && !devices[$('#mode-select .active').attr('id')]['#no-rotate']) {
        mobileAdaptive.rotate();
      }
       $('.mobile-shadow').click(function() {
        $('#mobile-adaptive-switch').removeClass('active');
        hideDevice();
        $('#mobile-adaptive-block').animate({'left': -210});
       });
    }

    function hideDevice() {
      mobileAdaptive.hide();
    }

    function UpdateModeSelect(mode)
    {
      $('#mobile-adaptive-block #mode-select div').removeClass('active');
      $('#' + mode).addClass('active');
      if (devices[mode]['#no-rotate']) {
        $('#orientation').hide();
      }
      else {
        $('#orientation').show();
      }
      var $os_select = $('#os');
      if (!devices[mode]['#no-os']) {
        $os_select.empty();
        $os_select.show();
        $.each(devices[mode], function(key, os) {
          $os_select.append($('<option/>').val(key).html(os['#title']));
        });
        UpdateDeviceList($('#os').val());
      }
      else
      {
        $os_select.hide();
        var $device_select = $('#device'); 
        $device_select.empty();
        $.each(devices[mode], function(key, device){
          if (key.indexOf('#') == -1) {
            $device_select.append($('<option/>').val(key).html(device['#name']));
          }
        });
        UpdateDevice();
      }
    }
    
    function UpdateDeviceList(os) {
      var mode = $('#mode-select .active').attr('id');
      var $device_select = $('#device'); 
        $device_select.empty();
        $.each(devices[mode][os], function(device){
          if (device.indexOf('#') == -1) {
            $device_select.append($('<option/>', {value: device, text: devices[mode][os][device]['#name'] }));
          }
        });
      UpdateDevice();
    }

    function isInitAllowed() {
      if (window.location.hash == '#no-mobile-adaptive') {
        return false;
      }
      return true;
    }

  });

  function MobileAdaptive(script_path, path) {
    this.blockId = 'mobile-adaptive';
    this.script_path = script_path;

    // Parse URL to gently adding the 'mobile' GET.
    attributes = new Array();
    get_symbol = path.indexOf('?');
    hash_symbol = path.indexOf('#');
    this.src = path;
    if (hash_symbol != '-1') {
      this.src = this.src.substring(0, this.src.indexOf('#'));
    }
    if (get_symbol != '-1') {
      get = this.src.substring(get_symbol);
      this.src = this.src.substring(0, get_symbol);
      attributes = (get.substr(1)).split('&');
    }
    attributes.push('mobile#no-mobile-adaptive');
    this.src += '?'
    for (attr in attributes) {
      if (attr != 0) {
        this.src += '&'
      }
      this.src += attributes[attr];
    }

    this.isInit = false;
    this.animationTime = 250;
    this.horizontal = false;
    this.device;


    this.init = function() {
      $('<div/>', {'id' : 'mobile-adaptive-switch'}).appendTo('body');
      $('<div/>', {'id' : 'mobile-adaptive-block'}).appendTo('body');
      $('<div/>', {'id' : 'mode-select'}).appendTo('#mobile-adaptive-block');
      $.each(devices, function(mode) {
        $('<div/>', {'id' : mode}).appendTo('#mode-select');
      });
      $('<div/>', {
        'id' : 'configs',
        'class' : 'active'
      }).appendTo('#mobile-adaptive-block');
      $('<div/>', {'id' : 'os-select-wrapper'}).appendTo('#configs');
      $('<div/>', {'id' : 'device-select-wrapper'}).appendTo('#configs');
      $('<select/>', {'id' : 'os'}).appendTo('#os-select-wrapper');
      $('<select/>', {'id': 'device'}).appendTo('#configs #device-select-wrapper');
      $('<div/>', {'id' : 'orientation'}).appendTo('#configs');
      $('<div/>', {
        'id' : 'label',
        'text' : 'Orientation'
      }).appendTo('#orientation');
      $('<div/>', {
        'id' : 'landscape',
        'class' : 'rotate',
        'title' : 'rotate',
        'text' : 'Landscape'
      }).appendTo('#orientation');
      $('<div/>', {
        'id' : 'portrait',
        'class' : 'rotate',
        'title' : 'rotate',
        'text' : 'Portrait'
      }).appendTo('#orientation');
    };
    
    this.getDeviceWrapper = function(device) {
      $mobile_wrap = $('<div/>', {'id' : this.blockId}).appendTo('body');
      $('<div/>', {'class' : 'mobile-shadow'}).appendTo($mobile_wrap);
      $device_bg = $('<div/>', {'class' : 'device-bg'}).appendTo($mobile_wrap);
      $('<img/>', {
        'class' : 'image',
        'src' : this.script_path + '/images/models/' + device.name + '.png'
      }).appendTo($device_bg);
      $('<div/>', {'class' : 'content'}).appendTo($device_bg);
    };

    this.show = function(device) {
      // Check if mobile adaptive emulator is active already.
      var mode = $('#mode-select .active').attr('id');
      var os = $('#os').val();
      localStorage['mobile_adaptive_mode'] = mode;
      localStorage['mobile_adaptive_os'] = os;
      localStorage['mobile_adaptive_device'] = device;
      if (this.isInit) {
        this.destroy();
      }
      this.device = new DeviceInfo(mode, os, device);
      // Init wrapper.
      this.getDeviceWrapper(this.device);
      // Init device emulator.
      this.initWindow();
      // Set sizes
      if (!this.horizontal) {
        $('#' + this.blockId).addClass(this.device.name).addClass(this.device.cls).find('.content').width(this.device.screenWidth).height(this.device.screenHeight - this.device.menuHeight);
        $('#' + this.blockId).find('.device-bg').width(this.device.outerWidth).height(this.device.outerHeight);
      }
      else {
      $('#' + this.blockId).addClass(this.device.name + '-horizontal').find('.content').width(this.device.screenHeight).height(this.device.screenWidth);
      $('#' + this.blockId).find('.device-bg').width(this.device.outerHeight).height(this.device.outerWidth - this.device.menuHeight);
      }
      this.isInit = true;
    };
    
    this.initWindow = function() {
      if (!this.horizontal) {
        var iframeHtml = '<iframe src="' + this.src
        + '" width="' + this.device.screenWidth
        + '" height="' + (this.device.screenHeight - this.device.menuHeight)
        + '"></iframe>';
      }
      else {
        var iframeHtml = '<iframe src="' + this.src
        + '" width="' + this.device.screenHeight
        + '" height="' + (this.device.screenWidth - this.device.menuHeight)
        + '"></iframe>';
      }
      $('#' + this.blockId).find('.content').html(iframeHtml);
    }
    
    this.hide = function() {
      this.destroy();
    }
    
    this.rotate = function() {
      var mobileObject = this;
      if (this.horizontal) {
        $('#' + this.blockId).find('.device-bg').rotate({
          angle: -90,
          animateTo: 0,
          duration: this.animationTime,
          callback: function() {
            $('#' + mobileObject.blockId).find('iframe').fadeOut('fast', function() {
              $(this).width(mobileObject.device.screenWidth)
              .height(mobileObject.device.screenHeight - mobileObject.device.menuHeight)
              .css('-webkit-transform', '');
                $('#' + mobileObject.blockId).find('.device-bg')
                .removeClass('horizontal')
                .width(mobileObject.device.outerWidth)
                .height(mobileObject.device.outerHeight);
              if (mobileObject.device.hasHorizontal) {
                $('#' + mobileObject.blockId).find('img')
                .attr('src', mobileObject.script_path + '/images/models/' + mobileObject.device.name + '.png');
              }
              $(this).fadeIn('fast');
            });
          }
        });
        this.horizontal = false;
      }
      else {
        $('#' + this.blockId).find('.device-bg').rotate({
          angle: 0,
          animateTo: -90,
          duration: this.animationTime,
          callback: function() {
            $('#' + mobileObject.blockId).find('iframe').fadeOut('fast', function() {
              $(this).width(mobileObject.device.screenHeight)
              .height(mobileObject.device.screenWidth - mobileObject.device.menuHeight)
              .css('-webkit-transform', 'rotate(90deg)');
              $('#' + mobileObject.blockId).find('.device-bg')
              .addClass('horizontal')
              .width(mobileObject.device.outerHeight);
              if (mobileObject.device.hasHorizontal) {
                $('#' + mobileObject.blockId).find('img')
                .attr('src', mobileObject.script_path + '/images/models/' + mobileObject.device.name + '-horizontal.png');
              }
              $(this).fadeIn('fast');
            });
          }
        });
        this.horizontal = true;
      }
    }
    
    this.destroy = function() {
      $('#' + this.blockId).remove();
      this.isInit = false;
      this.horizontal = false;
    }
  }

  function DeviceInfo(mode, os, device) {
    var device_information = new Object();
    device_information.name = device;
    if (devices[mode]['#no-os']) {
      $.each(devices[mode][device], function(key, val) {
        device_information[key] = val;
      });
    }
    else {
      $.each(devices[mode][os][device], function(key, val) {
        device_information[key] = val;
      });
    }
    return device_information;
  }

  function DevicesList()  {
  	devices = new Object();
    devices['phone'] = new Object();
    devices['phone']['ios'] = {
      '#title' : 'iPhone',
      'iphone-3g' : {
        '#name' : '3g',
        'screenWidth' : 320,
        'screenHeight' : 480,
        'menuHeight' : 20,
        'outerWidth' : 373,
        'outerHeight' : 718,
        'hasHorizontal' : true,
      }, 
      'iphone-4s' : {
        '#name' : '4s',
        'screenWidth' : 640,
        'screenHeight' : 960,
        'menuHeight' : 40,
        'outerWidth' : 759,
        'outerHeight' : 1485,
        'hasHorizontal' : true,
      },
      'iphone-5' : {
        '#name' : '5',
        'screenWidth' : 640,
        'screenHeight' : 1136,
        'menuHeight' : 42,
        'outerWidth' : 763,
        'outerHeight' : 1602,
        'hasHorizontal' : true,
      }
    };
    devices['phone']['android'] = {
      '#title' : 'Android',
      'android-240x320' : {
        '#name' : '240x320',
        'screenWidth' : 240,
        'screenHeight' : 320,
        'menuHeight' : 0,
        'outerWidth' : 284,
        'outerHeight' : 440,
        'hasHorizontal' : false,
      },
      'android-320x480' : {
        '#name' : '320x480',
        'screenWidth' : 320,
        'screenHeight' : 480,
        'menuHeight' : 0,
        'outerWidth' : 365,
        'outerHeight' : 599,
        'hasHorizontal' : false,
      },
      'android-480x640' : {
        '#name' : '480x640',
        'screenWidth' : 480,
        'screenHeight' : 640,
        'menuHeight' : 0,
        'outerWidth' : 545,
        'outerHeight' : 829,
        'hasHorizontal' : false,
      },
      'android-480x800' : {
        '#name' : '480x800',
        'screenWidth' : 480,
        'screenHeight' : 800,
        'menuHeight' : 0,
        'outerWidth' : 544,
        'outerHeight' : 977,
        'hasHorizontal' : false,
      },
      'android-640x960' : {
        '#name' : '640x960',
        'screenWidth' : 640,
        'screenHeight' : 960,
        'menuHeight' : 0,
        'outerWidth' : 727,
        'outerHeight' : 1193,
        'hasHorizontal' : false,
      },
      'android-720x1280' : {
        '#name' : '720x1280',
        'screenWidth' : 720,
        'screenHeight' : 1280,
        'menuHeight' : 0,
        'outerWidth' : 809,
        'outerHeight' : 1533,
        'hasHorizontal' : false,
      },
      'android-768x1024' : {
        '#name' : '768x1024',
        'screenWidth' : 768,
        'screenHeight' : 1024,
        'menuHeight' : 0,
        'outerWidth' : 856,
        'outerHeight' : 1318,
        'hasHorizontal' : false,
      },
      'android-800x1280' : {
        '#name' : '800x1280',
        'screenWidth' : 800,
        'screenHeight' : 1280,
        'menuHeight' : 0,
        'outerWidth' : 891,
        'outerHeight' : 1533,
        'hasHorizontal' : false,
      },
      'android-1200x1920' : {
        '#name' : '1200x1920',
        'screenWidth' : 1200,
        'screenHeight' : 1920,
        'menuHeight' : 0,
        'outerWidth' : 1285,
        'outerHeight' : 2267,
        'hasHorizontal' : false,
      }
    };
    devices['tablet'] = new Object();
    devices['tablet']['ipad'] = {
      '#title' : 'iPad',
      'ipad-1': {
        '#name' : 'iPad-1',
        'screenWidth' : 768,
        'screenHeight' : 1024,
        'menuHeight' : 24,
        'outerWidth' : 967,
        'outerHeight' : 1254,
        'hasHorizontal' : true,
      },
      'ipad-2': {
        '#name' : 'iPad-2',
        'screenWidth' : 768,
        'screenHeight' : 1024,
        'menuHeight' : 24,
        'outerWidth' : 969,
        'outerHeight' : 1258,
        'hasHorizontal' : true,
      },
      'ipad-3': {
        '#name' : 'iPad-3',
        'screenWidth' : 1536,
        'screenHeight' : 2048,
        'menuHeight' : 40,
        'outerWidth' : 1923,
        'outerHeight' : 2509,
        'hasHorizontal' : true,
      },
      'ipad-mini': {
        '#name' : 'iPad-mini',
        'screenWidth' : 768,
        'screenHeight' : 1024,
        'menuHeight' : 28,
        'outerWidth' : 870,
        'outerHeight' : 1289,
        'hasHorizontal' : true,
      }
    };
    devices['tablet']['android-tab'] = {
      '#title' : 'Android',
      'android-tab-800x480': {
        '#name' : '800x480',
        'screenWidth' : 480,
        'screenHeight' : 800,
        'menuHeight' : 0,
        'outerWidth' : 617,
        'outerHeight' : 939,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-800x600': {
        '#name' : '800x600',
        'screenWidth' : 600,
        'screenHeight' : 800,
        'menuHeight' : 0,
        'outerWidth' : 737,
        'outerHeight' : 939,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-960x640': {
        '#name' : '960x640',
        'screenWidth' : 640,
        'screenHeight' : 960,
        'menuHeight' : 0,
        'outerWidth' : 817,
        'outerHeight' : 1139,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-1024x600': {
        '#name' : '1024x600',
        'screenWidth' : 600,
        'screenHeight' : 1024,
        'menuHeight' : 0,
        'outerWidth' : 777,
        'outerHeight' : 1203,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-1024x768': {
        '#name' : '1024x768',
        'screenWidth' : 768,
        'screenHeight' : 1024,
        'menuHeight' : 0,
        'outerWidth' : 945,
        'outerHeight' : 1203,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-1280x768': {
        '#name' : '1280x768',
        'screenWidth' : 768,
        'screenHeight' : 1280,
        'menuHeight' : 0,
        'outerWidth' : 958,
        'outerHeight' : 1499,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-1280x800': {
        '#name' : '1280x800',
        'screenWidth' : 800,
        'screenHeight' : 1280,
        'menuHeight' : 0,
        'outerWidth' : 1017,
        'outerHeight' : 1499,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-1366x768': {
        '#name' : '1366x768',
        'screenWidth' : 768,
        'screenHeight' : 1366,
        'menuHeight' : 0,
        'outerWidth' : 985,
        'outerHeight' : 1584,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      },
      'android-tab-2560x1600': {
        '#name' : '2560x1600',
        'screenWidth' : 1600,
        'screenHeight' : 2560,
        'menuHeight' : 0,
        'outerWidth' : 1918,
        'outerHeight' : 2878,
        'hasHorizontal' : false,
        'cls' : 'android-tab',
      }
    };
    devices['monitor'] = {
      '#no-rotate' : true,
      '#no-os' : true,
      'monitor-macbook-air': {
        '#name' : 'Macbook-Air',
        'screenWidth' : 1445,
        'screenHeight' : 900,
        'menuHeight' : 0,
        'outerWidth' : 1628,
        'outerHeight' : 1135,
        'hasHorizontal' : false,
      },
      'monitor-macbook-pro': {
        '#name' : 'Macbook-Pro',
        'screenWidth' : 2556,
        'screenHeight' : 1600,
        'menuHeight' : 0,
        'outerWidth' : 2819,
        'outerHeight' : 1965,
        'hasHorizontal' : false,
      },
      'monitor-800-600': {
        '#name' : '800x600',
        'screenWidth' : 800,
        'screenHeight' : 600,
        'menuHeight' : 0,
        'outerWidth' : 866,
        'outerHeight' : 830,
        'hasHorizontal' : false,
      },
      'monitor-1600-900': {
        '#name' : '1600x900',
        'screenWidth' : 1600,
        'screenHeight' : 900,
        'menuHeight' : 0,
        'outerWidth' : 1740,
        'outerHeight' : 1378,
        'hasHorizontal' : false,
      }
    };
    return devices;
  }
})(jQuery);