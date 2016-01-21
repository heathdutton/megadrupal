(function($) {

/**
 * Create a DART object to handle tagging functionality
 */
Drupal.DART = {};

/**
 * Overridable settings.
 */
Drupal.DART.settings = {
  "writeTags": true
};

/**
 * Using document.write, add a DART tag to the page
 */
Drupal.DART.tag = function(tag) {
  tag = typeof(tag) == 'string' ? eval('(' + tag + ')') : tag;

  var tagname = tag.settings.options.method == 'adj' ? 'script' : 'iframe';
  var options = tag.settings.options.method == 'adj' ? 'type="text/javascript"' : 'frameborder="0" scrolling="no" width="' + tag.sz.split("x")[0] + '" height="' + tag.sz.split("x")[1] + '"';

  // Allow other modules to include js that can manipulate the tag object.
  var processed_tag = ($ !== undefined) ? $(document).triggerHandler('dart_tag_process', [tag]) : undefined;
  tag = processed_tag !== undefined ? processed_tag : tag;

  ad = '<' + tagname + ' ' + options + ' src="';
  ad += dart_url + "/";
  ad += tag.network_id !== '' ? tag.network_id + "/" : "";
  ad += tag.settings.options.method + "/";
  ad += tag.prefix + '.' + tag.site + "/" + tag.zone + ";";
  ad += this.keyVals(tag.key_vals);

  // Allow other modules to include js that can manipulate the concatenated tag string.
  rendered_ad = ($ !== undefined) ? $(document).triggerHandler('dart_tag_render', [ad]) : undefined;
  ad = rendered_ad !== undefined ? rendered_ad : ad; ad += '"></' + tagname + '>';

  if (Drupal.DART.settings.writeTags) {
    document.write(ad);
  }

  // console.log('-----------------'+tag.pos+'------------------');
  // console.log(tag);

  return ad;
};

/**
 * Format a key|val pair into a dart tag key|val pair.
 */
Drupal.DART.keyVal = function(key, val, useEval) {
  if (key != "<none>") {
    kvp  = key + "=";
    kvp += useEval ? eval(val) : val;
    kvp += key == "ord" ? "?" : ";";
  }
  else {
    kvp = useEval ? eval(val) : val;
  }

  return(kvp);
};

/**
 * Loop through an object and create kay|val pairs.
 *
 * @param vals
 *   an object in this form:
 *   {
 *     key1 : {{val:'foo', eval:true}, {val:'foo2', eval:false}}
 *     key2 : {{val:'bar', eval:false}},
 *     key3 : {{val:'foobar', eval:true}}
 *   }
 */
Drupal.DART.keyVals = function(vals) {
  var ad = '';
  for(var key in vals) {
    value = vals[key];
    for(var val in value) {
      v = value[val];
      ad += this.keyVal(key, v['val'], v['eval']);
    }
  }
  return ad;
};


/**
 * If there are tags in the loadLastTags, then load them where they belong.
 */
Drupal.DART.display_ads = function () {
  ord = Math.round(Math.random()*1000000000000);
    if (typeof(Drupal.DART.settings.loadLastTags) == 'object') {
    $('.dart-tag.dart-processed').each(function () {
      $(this).removeClass('dart-processed');
      $(this).html('');
    });
    var init = false;
    for (var tag in Drupal.DART.settings.loadLastTags) {
      // variables for background ads may be defined in late loaded scripts. Load bg ad if needed.
      if (Drupal.DART.settings.loadLastTags.hasOwnProperty(tag) && tag != null) {
        (function(tag) {
          var name = tag;
          var scriptTag = Drupal.DART.tag(Drupal.DART.settings.loadLastTags[name]);
          if (typeof(postscribe) == 'function') {
            postscribe($('.dart-name-' + name), scriptTag, function () {
              Drupal.DART.loadBgAd(Drupal.settings.DART.bgAdVars);
              $('.dart-name-' + name).addClass('dart-processed');
            });
          }
          else if (typeof(_this.writeCapture) == 'function') {
            $('.dart-name-' + name).writeCapture().append(scriptTag, function () {
                Drupal.DART.loadBgAd(Drupal.settings.DART.bgAdVars);
            }).addClass('dart-processed');
          }
        }(tag));
      }
    }
  }
}

/**
 * Load the background ad as served by DART.
 */
Drupal.DART.loadBgAd = function(bgAdVars) {
  //ensure ads are loaded only once on the page
  if (!Drupal.DART.settings.bgAdLoaded && typeof bgAdVars != 'undefined') {
    var bgAdCSS = {};
    if (window[bgAdVars.bgImg] != undefined) {
      bgAdCSS['background-image'] = 'url(' + window[bgAdVars.bgImg] + ')';
    }
    if (window[bgAdVars.bgColor] != undefined) {
      bgAdCSS['background-color'] = window[bgAdVars.bgColor];
    }
    if (window[bgAdVars.bgRepeat] != undefined) {
      bgAdCSS['background-repeat'] = window[bgAdVars.bgRepeat];
    }
    $(bgAdVars.selector).css(bgAdCSS);

    if (window[bgAdVars.clickThru] != undefined) {
      $(bgAdVars.selector).addClass('background-ad');
      $(bgAdVars.selector).click(function (e) {
      if(e.target != this) return;
        window.open(window[bgAdVars.clickThru]);
      });
    }

    //don't try to load again
    if (window[bgAdVars.bgImg] != undefined) {
      Drupal.DART.settings.bgAdLoaded = true;
    }
  }
};



/**
 * Display Ads.
 */
Drupal.behaviors.DART = {
  attach: function(context) {
    Drupal.DART.display_ads();
    Drupal.DART.loadBgAd(Drupal.settings.DART.bgAdVars);
  }
};

})(jQuery);

