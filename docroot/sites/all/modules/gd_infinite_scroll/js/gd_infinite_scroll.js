// $Id:

(function ($) {
Drupal.behaviors.gd_infinite_scroll = {
  /**
   * Callback document loaded.
   */
  attach:function() {
    // Make sure that autopager plugin is loaded.
    var gd_settings = Drupal.settings.gd_infinite_scroll;
    if (typeof gd_settings == 'object') {
      for (var key in gd_settings) {
        var $content_selector  = $(gd_settings[key].content_selector);
        if ($content_selector[0] && !$content_selector.hasClass('gd-infinite-scroll-initialized')) {
          $content_selector.addClass('gd-infinite-scroll-initialized');
          console.log('attach');
          this.init_gd_infinite_scroll(key, gd_settings[key]);
        }
      }
    }
    
  },
  
  /**
   * This functions initialize the autopager.
   */
  init_gd_infinite_scroll: function(autopager_id, settings) {
    var content_selector  = settings.content_selector;
    var items_selector    = settings.content_selector + ' ' + settings.items_selector;
    var pager_selector    = settings.pager_selector;
    var next_selector     = settings.next_selector;
    var img_location      = settings.content_selector;
    var img_path          = settings.img_path;
    var load_more         = settings.load_more;
    var load_more_markup  = settings.load_more_markup;
    var ajax_pager         = settings.ajax_pager;
    var img               = '<div class="gd_infinite_scroll-ajax-loader"><img src="' + img_path + '" alt="' + Drupal.t('Loading') + '..."></div>';
    var $this = this;
    if (!$(content_selector)[0]) {
      return;
    }
    
    var autoloading = true;
    var load_more_button = $('<a></a>');
    if (load_more) {
      autoloading = false;
      load_more_button = $(load_more_markup);
    }
    if (ajax_pager) {
      // Ajax pager
      autoloading = false;
      $(pager_selector + ' a').click(function(e) {
        var href = $(this).attr('href');
        $.get(href, function (data) {
          var nextPage = $('<div/>').append(data.replace(/<script(.|\s)*?\/script>/g, "")),
            nextContent = nextPage.find(content_selector);
          $(content_selector).replaceWith(nextContent);
          Drupal.attachBehaviors($(content_selector)[0]);
        });
        e.preventDefault();
      });
    }
    else {
      // Autopager
      $(pager_selector).hide();
  
      this.autopager(
        autopager_id,
        {
          autoLoad: autoloading,
          appendTo: content_selector,
          content: items_selector,
          link: next_selector,
          page: 0,
          start: function() {
            $(img_location).after(img);
          },
          load: function(current, next) {
            $(content_selector).next('div.gd_infinite_scroll-ajax-loader').remove();
            Drupal.attachBehaviors(this);
            if (load_more && !next.url) {
              load_more_button.hide();
            }
          }
        }
      );
    }
    if (!load_more && !ajax_pager && $(items_selector)[0]) {
      // Trigger autoload if content height is less than doc height already
      var prev_content_height = $(content_selector).height();
      do {
        var last = $(items_selector).filter(':last');
        if(last.offset().top + last.height() < $(document).scrollTop() + $(window).height()) {
          last = $(items_selector).filter(':last');
          $this.load(autopager_id);
        }
        else {
          break;
        }
      }
      while ($(content_selector).height() > prev_content_height);
    }            
    else if ($(next_selector)[0]){        
      load_more_button.addClass('gd-infinite-scroll-load-more')
      .appendTo($(img_location).parent())
      .click(function(e) {
        // do load
        $this.load(autopager_id);
        e.preventDefault();
      });
    }
  },
  state: {},
  _optionsDefault: {
    autoLoad: true,
    page: 1,
    content: '.content',
    link: 'a[rel=next]',
    insertBefore: null, 
    appendTo: null, 
    start: function() {},
    load: function() {},
    disabled: false
  },
  setState: function(autopager_id, values) {
    for (var key in values) { 
      this.state[autopager_id][key] = values[key]; 
    }
  },
  autopager: function(autopager_id, options) {
    var _options = $.extend({}, this._optionsDefault, options);
    var _content = $(_options.content).filter(':last');
    if (_content.length) {
      if (!_options.insertBefore && !_options.appendTo) {
        var insertBefore = _content.next();
        if (insertBefore.length) {
          this.setOption('insertBefore', insertBefore);
        } 
        else {
          this.setOption('appendTo', _content.parent());
        }
      }
    }
    this.state[autopager_id] = {
      content: _content,
      active: false,
      options: {},
    },
    this.option(autopager_id, _options);
    this.setUrl(autopager_id);
  },
  loadOnScroll: function(autopager_id, e) {
    var content = this.state[autopager_id]['content'];
    if (content.offset().top + content.height() < $(document).scrollTop() + $(window).height()) {
      this.load(autopager_id);
    }
  },
  load: function(autopager_id) {
    if (this.state[autopager_id]['active'] || 
        !this.state[autopager_id]['nextUrl'] || 
        this.option(autopager_id, 'disabled')) {
      return;
    }
    this.setState(autopager_id, {active: true});
   
    var fn = this.option(autopager_id, 'start'); 
    fn(this.currentHash(autopager_id), this.nextHash(autopager_id));
    $.get(this.state[autopager_id]['nextUrl'], this.insertContent.bind(this, autopager_id));
    return this;
  },
  insertContent: function(autopager_id, res) {
    var _options = this.state[autopager_id]['options'],
      nextPage = $('<div/>').append(res.replace(/<script(.|\s)*?\/script>/g, "")),
      nextContent = nextPage.find(_options.content); 

    this.setOption(autopager_id, 'page', _options.page + 1);
    this.setUrl(autopager_id, nextPage);
    if (nextContent.length) {
      if (_options.insertBefore) {
        nextContent.insertBefore(_options.insertBefore);
      } else {
        nextContent.appendTo(_options.appendTo);
      }
      _options.load.call(nextContent.get(), this.currentHash(autopager_id), this.nextHash(autopager_id));
      this.state[autopager_id]['content'] = nextContent.filter(':last');
    }
    this.state[autopager_id]['active'] = false;
  },
  currentHash: function(autopager_id) {
    var _options = this.state[autopager_id]['options'];
    return {
      page: _options.page,
      url: this.state[autopager_id]['currentUrl']
    };
  },
  nextHash: function(autopager_id) {
    var _options = this.state[autopager_id]['options'];
    return {
      page: _options.page + 1,
      url: this.state[autopager_id]['nextUrl']
    };
  },
  setUrl: function(autopager_id, context) {
    var values = {};
    values.currentUrl = this.state[autopager_id]['nextUrl'] || window.location.href;
    values.nextUrl = $(this.option(autopager_id, 'link'), context).attr('href');
    this.setState(autopager_id, values);
  },
  option: function(autopager_id, key, value) {
    var _options = key;
    var $this = this;
    if (typeof key === "string") {
      if (value === undefined) {
        return this.state[autopager_id]['options'][key];
      }
      _options = {};
      _options[key] = value;
    }
 
    $.each(_options, function(key, value) {
      $this.setOption(autopager_id, key, value);
    });
    return this;
  },
  setOption: function(autopager_id, key, value) {
    switch (key) {
      case 'autoLoad':
        if (value && !this.state[autopager_id]['options']['autoLoad']) {
          $(window).scroll(this.loadOnScroll.bind(this, autopager_id));
        } else if (!value && this.state[autopager_id]['options']['autoLoad']) {
          $(window).unbind('scroll', this.loadOnScroll);
        }
        break;
      case 'insertBefore':
        if (value) {
          this.state[autopager_id]['options']['appendTo'] = null;
        }
        break
      case 'appendTo':
        if (value) {
          this.state[autopager_id]['options']['insertBefore'] = null;
        }
        break
    }
    this.state[autopager_id]['options'][key] = value;
  },

  enable: function(autopager_id) {
    this.setOption(autopager_id, 'disabled', false);
    return this;
  },

  disable: function(autopager_id) {
    this.setOption(autopager_id, 'disabled', true);
    return this;
  }
}

})(jQuery);
