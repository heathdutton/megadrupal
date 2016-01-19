Drupal.emediate = Drupal.emediate || {};

(function ($) {

Drupal.emediate.banner = function(idx) {
  var content_unit = this.settings.content_units[idx]
  if (content_unit.iframe) {
    this.bannerIframe(idx)
  }
  else if (typeof content_unit.func === "function") {
    content_unit.func()
  }
}

Drupal.emediate.bannerIframe = function(idx) {
  var content_unit = this.settings.content_units[idx]
  if (content_unit.camp) {
    // Generate emediate query parameters
    var query = $.extend({}, this.settings.query)
    query['no'] = content_unit.no
    query['camp'] = content_unit.camp
    var url = this.settings.eas_server + "/eas?target=_parent;cre=mu;cu=" + content_unit.cuid + ";" + this.generateQuery(query).join(';')
    document.write('<iframe src="' + url + '" width="' + content_unit.width + '" height="' + content_unit.height + '" scrolling="no" frameborder="0" marginheight="0" marginwidth="0"></iframe>')
  }
}

Drupal.emediate.bannerInline = function(cuid) {
  // Generate emediate query parameters
  var query = $.extend({}, this.settings.query)
  query['time'] = this.timestamp
  var url = this.settings.eas_server + "/eas?target=_blank;js=y;cre=mu;cu=" + cuid + ";" + this.generateQuery(query).join(';')
  document.write('<script language="JavaScript" src="' + url + '"></script>');
}

Drupal.emediate.generateQuery = function(query) {
  var q = $.extend({}, query)
  $.event.trigger('EmediateQueryPreAlter', q)

  var qs = []
  $.each(q, function(key, value) {
    if (typeof value == 'undefined' || value == null) {
      value = ''
    }
    qs.push(key + '=' + value)
  })

/*
  // @todo Move this code to a "post-alter" event?
  if (this.settings.nuggad && typeof Drupal.nuggAd !== 'undefined') {
    qs.push(Drupal.nuggAd.prepareCookieString())
  }
*/

  return qs
}

Drupal.emediate.initialize = function() {
  this.settings = Drupal.settings.emediate
  // Do EAS_init() and move global data here
  this.EAS_init(this.settings.content_units, false)
  this.EAS_init(this.settings.content_units, true)
  this.counter = 1
  this.settings.eas_server = EAS_server
}

Drupal.emediate.EAS_init = function(content_units, exclude_campaign) {
  var cuids = []
  $.each(content_units, function(name, value) {
    if (value.exclude_campaign == exclude_campaign) {
      cuids.push(value.cuid)
    }
  })

  // Generate emediate query parameters
  var query = $.extend({}, this.settings.query)
  query['time'] = this.timestamp
  query['exclCamp'] = exclude_campaign ? 1 : 0
  query = this.generateQuery(query)

  if (cuids.length == 0) {
    return
  }
  // Load the content units
  EAS_init(cuids.join(','), query.join(';'))

  $.event.trigger('EmediatePostInit', this)

  // Prepare move of global data
  this.content_units = content_units
}

Drupal.emediate.startMoveGlobalData = function() {
  var self = this
  $.each(this.content_units, function(idx, value) {
    self.moveGlobalData(value) 
  })
}

Drupal.emediate.moveGlobalData = function(content_unit) {
  var suffix = ''
  if (content_unit.exclude_campaign) {
    suffix = '_' + this.counter
    this.counter++
  }

  var idx = content_unit.cuid + suffix

  content_unit.no       = window['EAS_found_cre_' + idx]
  content_unit.camp     = window['EAS_found_camp_' + idx]
  content_unit.width    = window['EAS_found_width_' + idx]
  content_unit.height   = window['EAS_found_height_' + idx]
  content_unit.func     = window['EAS_' + idx]
}

})(jQuery);

Drupal.emediate.timestamp = new Date().getTime();
Drupal.emediate.bannersProcessed = 0;

