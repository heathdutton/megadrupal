// Set up namespace and some state.
var mkdru = {
  // Variables
  active: false,
  callbacks: [],
  pz2: null,
  totalRec: 0,
  pagerRange: 6,
  defaultState: {
    page: 1,
    perpage: 20,
    sort: 'relevance',
    query:'',
    recid:null
  },
  state: {},
  form: {},
  sourceNameCache: {},
  realm: ''
};

// Wrapper for jQuery
(function ($) {
mkdru.settings = $.parseJSON(Drupal.settings.mkdru.settings);

// Reference for external use
mkdru.facets = mkdru.settings.facets;

// BBQ has no handy way to remove params without changing the hash.
// This takes an object to add and an array of keys to delete.
mkdru.hashAddDelMany = function (add, del) {
  var newHash = $.deparam.fragment();
  if (typeof(add) === 'object')
    $.extend(newHash, add);
  if ($.isArray(del))
    for (var i=0; i < del.length; i++)
      if (newHash[del[i]] !== 'undefined')
        delete newHash[del[i]];
  return $.param.fragment("#", newHash);
}

// It's sometimes cumbersome that object literals can't take variable keys.
mkdru.hashAddDelOne = function (key, value, del) {
  var toAdd;
  var toDel;
  if (key && value) {
    var toAdd = {};
    toAdd[key] = value;
  }
  if (del) {
    var toDel = [];
    toDel.push(del);
  }
  return mkdru.hashAddDelMany(toAdd, toDel);
}

// pz2.js event handlers:
mkdru.pz2Init = function () {
  if (mkdru.state.query) {
    // search will issue stat and termlist if the callbacks are enabled
    mkdru.search();
  }
};

mkdru.pz2Show = function (event, data) {
  mkdru.totalRec = data.merged;
  $('.mkdru-pager').html(mkdru.generatePager());
  $('.mkdru-counts').html(Drupal.theme('mkdruCounts',
                                      data.num > 0 ? data.start + 1 : 0,
                                      data.start + data.num,
                                      data.merged,
                                      data.total));
  var html = "";
  for (var i = 0; i < data.hits.length; i++) {
    html += Drupal.theme('mkdruResult', data.hits[i],
      i + 1 + mkdru.state.perpage * (mkdru.state.page - 1),
      "#" + $.param.fragment($.param.fragment(
        window.location.href, {recid: data.hits[i].recid})) + "\n"
    );
  }
  $('.mkdru-result-list').html(html);
  if (mkdru.state.recid) {
    mkdru.pz2.record(mkdru.state.recid);
  }
  else {
    $('.mkdru-results').show();
  }
};

mkdru.pz2Status = function (event, data) {
  $('.mkdru-status').html(Drupal.theme('mkdruStatus', data.activeclients, data.clients));
};

mkdru.compareIgnoreCase = function (value1, value2) {
  var lower1 = value1.toLowerCase();
  var lower2 = value2.toLowerCase();
  return lower1 == lower2;
};

// check if obj[key] is defined and contains a case insensitive match to value
// or an array with an element that matches
mkdru.contains = function (obj, key, value) {
  if (obj[key] !== undefined) {
    if ($.isArray(obj[key])) {
      for (var i=0; i<obj[key].length; i++) {
        if (mkdru.compareIgnoreCase(obj[key][i], value)) {
          return true;
        }
      }
    } else {
      return mkdru.compareIgnoreCase(obj[key], value);
    }
  }
  return false;
}

mkdru.pz2Term = function (event, data) {
  // map all facets against selected, for simple rendering
  var hash = $.deparam.fragment();
  var limits = {};
  for (var key in hash) {
    if (key.indexOf('limit') == 0 && hash[key]) {
      // On IE we apparently need to cast this despite deparam.fragment
      // not coercing types by default.
      limits[key.substr(6)] = String(hash[key]).split(/;+/);
    }
  }
  for (var facet in mkdru.facets) {
    var terms = data[mkdru.facets[facet].pz2Name];
    var selectedTerms = [];
    var otherTerms = [];
    var displayTerms = [];
    var handled = {};
    var selections = limits[facet];
    var max = mkdru.facets[facet].max;
    for (var i=0; terms && i < terms.length; i++) {
      var term = terms[i];
      // See below re: the source facet is different
      if (facet === 'source') {
        var value = term.id;
        mkdru.sourceNameCache[term.id] = term.name;
      }
      else var value = $.trim(term.name);
      if (mkdru.contains(limits, facet, value)) { //enabled
        term.toggleLink = mkdru.removeLimit(facet, value);
        term.selected = true;
        handled[value.toLowerCase()] = true;
        selectedTerms.push(term);
      }
      else { //disabled
        term.toggleLink = mkdru.addLimit(facet, value);
        term.selected = false;
        otherTerms.push(term);
      }
    }
    for (var j=0; selections && j < selections.length; j++) {
      var selection = selections[j];
      // Generate terms for limits that didn't show up in the response
      if (handled[selection.toLowerCase()] !== true) {
        var newTerm = {
          selected: true,
          toggleLink: mkdru.removeLimit(facet, selection),
        }
        // Source isn't actually a facet and is limited by id rather
        // than value. Via a different operator than the others. And so
        // we can't recover the name from a limit in the URL. If it was
        // seen this session we may have a name, otherwise we can parse
        // one out of the id.
        if (mkdru.sourceNameCache[selection]) {
          newTerm.name = mkdru.sourceNameCache[selection];
        }
        else newTerm.name = selections[j].replace(/.*[\/\\]/, "").replace(/\?.*/, '');
        selectedTerms.push(newTerm);
      }
    }
    if (selectedTerms.length < max) {
      displayTerms = otherTerms.slice(0, max - selectedTerms.length);
      displayTerms = displayTerms.concat(selectedTerms);
    }
    else displayTerms = selectedTerms;
    displayTerms.sort(function (a, b) {
      na = Number(a.freq);
      nb = Number(b.freq);
      if (isNaN(na)) return 1;
      if (isNaN(nb)) return -1;
      return nb - na;
    });
    $('.mkdru-facet-' + facet).html(
        Drupal.theme('mkdruFacet', displayTerms, facet, max, selections));
  }
};

mkdru.pz2Record = function (event, data) {
  clearTimeout(mkdru.pz2.showTimer);
  $('.mkdru-results').hide();
  $('.mkdru-detail').html(Drupal.theme('mkdruDetail', data, mkdru.hashAddDelOne(null, null, 'recid')));
  $('.mkdru-detail').show();
  clearTimeout(mkdru.pz2.recordTimer);
};



// State and URL handling

// populate state from current window's hash string
mkdru.stateFromHash = function () {
  mkdru.state = $.extend({}, mkdru.defaultState, $.deparam.fragment());
};

// set current window's hash string from state
mkdru.hashFromState = function () {
  // only include non-default settings in the URL
  var alteredState = {};
  for (var key in mkdru.defaultState) {
    if (mkdru.state[key] != mkdru.defaultState[key]) {
      alteredState[key] = mkdru.state[key];
    }
  }
  $.bbq.pushState(alteredState, 2);
};

mkdru.hashChange = function () {
  // return to top of page
  window.scrollTo(0,0);
  // do we need to restart the search?
  var searchTrigger = false;
  // shallow copy of state so we can see what changed.
  var oldState = $.extend({}, mkdru.state);
  mkdru.stateFromHash();
  mkdru.form.fromState();
  // only have to compare values since all keys are initialised
  for (key in mkdru.state) {
    var changed = (mkdru.state[key] != oldState[key]);
    if (key.substring(0,5) === 'limit' && changed)
      searchTrigger = true;
    if (key === 'page' && changed)
      mkdru.pz2.showPage(mkdru.state.page-1);
    if (key === 'query' && changed)
      searchTrigger = true;
  }
  if (searchTrigger)
    mkdru.search();
  // request for record detail
  if (mkdru.state.recid && (mkdru.state.recid != oldState.recid)) {
    mkdru.pz2.record(mkdru.state.recid);
  }
  else {
    $('.mkdru-detail').hide();
    $('.mkdru-results').show();
  }
};

// return link to limit facet
mkdru.addLimit = function (facet, limit) {
  var newHash = $.deparam.fragment();
  delete newHash['page'];
  if ((typeof(newHash['limit_' + facet]) === 'undefined')
       || !mkdru.facets[facet].multiLimit) {
    newHash['limit_' + facet] = limit;
  }
  else {
    newHash['limit_' + facet] += ';' + limit;
  }
  return $.param.fragment("#", newHash);
};

// return link to remove limit from facet
mkdru.removeLimit = function (facet, limit) {
  var newHash = $.deparam.fragment();
  delete newHash['page'];
  if (!newHash['limit_' + facet].indexOf(';')
      || !mkdru.facets[facet].multiLimit) {
    delete newHash['limit_' + facet];
  }
  else {
    var limits = newHash['limit_' + facet].split(';');
    for (var i = 0; i < limits.length; i++) {
      if (limits[i] == limit) {
        limits.splice(i, 1);
        if (limits.length < 1)
          delete newHash['limit_' + facet];
        else
          newHash['limit_' + facet] = limits.join(';');
        break;
      }
    }
  }
  return $.param.fragment("#", newHash);
};

// form submit handler
mkdru.submitQuery = function () {
  // new query, back to defaults (shallow copy)
  mkdru.state = $.extend({}, mkdru.defaultState);
  mkdru.form.updateState();
  mkdru.hashFromState();
  mkdru.search();
  mkdru.active = true;
  return false;
};

// Find the sort order we want to use based on settings and state
mkdru.sortOrder = function () {
  if (mkdru.settings.disable_ranking == 1) {
    return "position:1";
  }
  return mkdru.state.sort;
};

// criteria drop-down (perpage, sort) handler
mkdru.submitCriteria = function () {
  mkdru.form.updateCriteria();
  //search is not ON, do nothing
  if (!mkdru.active) return false;
  // pages mean different things now
  mkdru.state.page = 1;
  mkdru.hashFromState();
  mkdru.pz2.show(0, mkdru.state.perpage, mkdru.sortOrder());
  return false;
};

mkdru.search = function () {
  var filter = null;
  var limit = null;
  var limits = [];
  // prepare filter and limit parameters for pz2.search() call
  for (var facet in mkdru.facets) {
    // facet is limited
    if (mkdru.state['limit_' + facet]) {
      // source facet in the filter parameter, everything else in limit
      if (facet == "source") {
        filter = 'pz:id=' + mkdru.state.limit_source;
      } else {
        var facetLimits = mkdru.state['limit_' + facet].split(/;+/);
        for (var i = 0; i < facetLimits.length; i++) {
          // Escape backslashes, commas, and pipes as per docs
          var facetLimit = facetLimits[i];
          facetLimit = facetLimit.replace(/\\/g, '\\\\')
                                 .replace(/,/g, '\\,')
                                 .replace(/\|/g, '\\|');
          limits.push(mkdru.facets[facet]['pz2Name'] + '=' + facetLimit);
        }
      }
    }
  }
  if (limits.length > 0) limit = limits.join(',');
  mkdru.pz2.search(mkdru.state.query, mkdru.state.perpage,
                   mkdru.sortOrder(), filter, null, {limit: limit});
  mkdru.active = true;
};

mkdru.generatePager = function () {
  // cast page parameter to numeric so we can add to it
  if (typeof mkdru.state.page == "string") {
    mkdru.state.page = Number(mkdru.state.page);
  }
  var total = Math.ceil(mkdru.totalRec / mkdru.state.perpage);
  var first = (mkdru.state.page - mkdru.pagerRange > 0)
      ? mkdru.state.page - mkdru.pagerRange : 1;
  var last = first + 2 * mkdru.pagerRange < total
      ? first + 2 * mkdru.pagerRange : total;
  var prev = null;
  var next = null;
  var pages = [];

  if ((mkdru.state.page - 1) >= first) {
    prev = "#" + $.param.fragment($.param.fragment(
               window.location.href, {page: mkdru.state.page - 1}))
  }
  if ((mkdru.state.page + 1) <= total) {
    next = "#" + $.param.fragment($.param.fragment(
               window.location.href, {page: mkdru.state.page + 1}))
  }

  for (var i = first; i <= last; i++) {
    pages.push("#" + $.param.fragment($.param.fragment(
               window.location.href, {page: i})));
  }

  return Drupal.theme('mkdruPager', pages, first, mkdru.state.page,
                      total, prev, next);
};

// check if we're authenticated, attempt to re-auth if we're not
mkdru.authCheck = function (successCb, failCb) {
  var params = {"command":"auth", "action" : "check"};
  var req = new pzHttpRequest(this.servicePath);
  var context = this;
  req.get(params, function (data) {
    if (Element_getTextContent(data.getElementsByTagName("status")[0]) === "OK") {
      if (typeof successCb == "function") successCb();
    } else {
      mkdru.auth(successCb, failCb);
    }
  });
};

mkdru.auth = function (successCb, failCb) {
  var user = mkdru.settings.sp_user;
  var pass = mkdru.settings.sp_pass;
  var params = {};
  params['command'] = 'auth';
  if (user && pass) {
    params['action'] = 'login';
    params['username'] = user;
    params['password'] = pass;
  } else {
    params['action'] = 'ipauth';
  }
  var authReq = new pzHttpRequest(mkdru.settings.pz2_path,
    function (err) {
      alert(Drupal.t("Authentication against metasearch gateway failed: ") + err);
    }
  );
  authReq.get(params,
    function (data) {
      var s = data.getElementsByTagName('status');
      if (s.length && Element_getTextContent(s[0]) == "OK") {
        mkdru.realm = data.getElementsByTagName('realm');
        mkdru.pz2Init();
        if (typeof successCb == "function") successCb();
      } else {
        if (typeof failCb == "function") failCb();
        else alert(Drupal.t("Failed to authenticate against the metasearch gateway"));
      }
    }
  );
};

mkdru.init = function () {
  // generate termlist for pz2.js and populate facet limit state
  var termlist = [];
  for (var key in mkdru.facets) {
    termlist.push(mkdru.facets[key].pz2Name);
    mkdru.defaultState['limit_' + key] = null;
  }

  $(document).bind('mkdru.onrecord', mkdru.pz2Record);
  $(document).bind('mkdru.onshow', mkdru.pz2Show);
  $(document).bind('mkdru.onstat', mkdru.pz2Status);
  $(document).bind('mkdru.onterm', mkdru.pz2Term);
  $(document).bind('mkdru.oninit', mkdru.pz2Init);

  var pz2Params = {
    "pazpar2path": mkdru.settings.pz2_path,
    "termlist": termlist.join(','),
    "usesessions" : !mkdru.settings.is_service_proxy,
    "autoInit": false,
    "showtime": 500, //each timer (show, stat, term, bytarget) can be specified this way
    "showResponseType": mkdru.showResponseType,
    "onshow": function (data) {
      $(document).trigger('mkdru.onshow', [data]);
    },
    "oninit": function () {
      $(document).trigger('mkdru.oninit');
    },
    "onstat": function (data) {
      $(document).trigger('mkdru.onstat', [data]);
    },
    "onterm": function (data) {
      $(document).trigger('mkdru.onterm', [data]);
    },
    "onrecord": function (data) {
      $(document).trigger('mkdru.onrecord', [data]);
    },
  };
  if (mkdru.settings.mergekey) pz2Params.mergekey = mkdru.settings.mergekey;
  if (mkdru.settings.rank) pz2Params.rank = mkdru.settings.rank;
  if (mkdru.settings.sp_server_auth) pz2Params.pazpar2path += ';jsessionid=' + Drupal.settings.mkdru.jsessionid;
  mkdru.pz2 = new pz2(pz2Params);
  mkdru.pz2.showFastCount = 1;

  // callback for access to DOM and pz2 object pre-search
  for (var i=0; i < mkdru.callbacks.length; i++) {
    mkdru.callbacks[i]();
  }

  if (typeof(Drupal.settings.mkdru.state) === "object") {
    // initialise state with properties from the hash in the URL taking
    // precedence over initial values passed in from embedding and
    // with defaults filling in the gaps
    mkdru.state = $.extend({}, mkdru.defaultState, Drupal.settings.mkdru.state, $.deparam.fragment());
    mkdru.hashFromState();
  } else {
    // initialise state to hash string or defaults
    mkdru.stateFromHash();
  }

  // update UI to match
  mkdru.form.fromState();

  if (mkdru.settings.is_service_proxy) {
    // SP doesn't trigger the init callback
    if (!mkdru.settings.sp_server_auth) mkdru.auth();
    else mkdru.pz2Init();
  } else {
    mkdru.pz2.init();
  }
};

$(document).ready(function () {
  mkdru.form.init();
  mkdru.init();
});
})(jQuery);
