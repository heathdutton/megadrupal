/**
 * Test KSBServicesModelThingie operation.
 *
 * Execute in browser console.
 */


// Operation: index.
console.clear();
jQuery.ajax({
  url: '/service/restmini-example/example/thingie/',
  type: 'GET',
  data: null,
  dataType: 'json',
  cache: false,
  success: function(oResp, textStatus, jqXHR) {
    if (window.inspect) {
      inspect([jqXHR.status, oResp]);
    }
    else {
      console.log(jqXHR.status + "\n" + oResp);
    }
  },
  error: function(jqXHR, textStatus, errorThrown) {
    if (window.inspect) {
      inspect([jqXHR.status, jqXHR.responseText]);
    }
    else {
      console.log(jqXHR.status + "\n" + jqXHR.responseText);
    }
  }
});

// Operation: create.
console.clear();
jQuery.ajax({
  url: '/service/restmini-example/example/thingie',
  type: 'POST',
  data: {
    properties: {
      name: 'first',
      color: 'green'
    }
  },
  dataType: 'json',
  cache: false,
  success: function(oResp, textStatus, jqXHR) {
    if (window.inspect) {
      inspect([jqXHR.status, oResp]);
    }
    else {
      console.log(jqXHR.status + "\n" + oResp);
    }
  },
  error: function(jqXHR, textStatus, errorThrown) {
    if (window.inspect) {
      inspect([jqXHR.status, jqXHR.responseText]);
    }
    else {
      console.log(jqXHR.status + "\n" + jqXHR.responseText);
    }
  }
});
// Operation: create more.
console.clear();
jQuery.ajax({
  url: '/service/restmini-example/example/thingie',
  type: 'POST',
  data: {
    instances: [
      {
        name: 'second',
        color: 'blue - is this/url safe?'
      }
    ]
  },
  dataType: 'json',
  cache: false,
  success: function(oResp, textStatus, jqXHR) {
    if (window.inspect) {
      inspect([jqXHR.status, oResp]);
    }
    else {
      console.log(jqXHR.status + "\n" + oResp);
    }
  },
  error: function(jqXHR, textStatus, errorThrown) {
    if (window.inspect) {
      inspect([jqXHR.status, jqXHR.responseText]);
    }
    else {
      console.log(jqXHR.status + "\n" + jqXHR.responseText);
    }
  }
});

// Operation: update. Parameters placed in JSON serialized GET parameter.
console.clear();
jQuery.ajax({
  url: '/service/restmini-example/example/thingie/2',
  type: 'PUT',
  data: {
    properties: {
      color: 'orange'
    }
  },
  dataType: 'json',
  cache: false,
  success: function(oResp, textStatus, jqXHR) {
    if (window.inspect) {
      inspect([jqXHR.status, oResp]);
    }
    else {
      console.log(jqXHR.status + "\n" + oResp);
    }
  },
  error: function(jqXHR, textStatus, errorThrown) {
    if (window.inspect) {
      inspect([jqXHR.status, jqXHR.responseText]);
    }
    else {
      console.log(jqXHR.status + "\n" + jqXHR.responseText);
    }
  }
});

// Operation: delete.
console.clear();
jQuery.ajax({
  url: '/service/restmini-example/example/thingie/1',
  type: 'DELETE',
  data: null,
  dataType: 'json',
  cache: false,
  success: function(oResp, textStatus, jqXHR) {
    if (window.inspect) {
      inspect([jqXHR.status, oResp]);
    }
    else {
      console.log(jqXHR.status + "\n" + oResp);
    }
  },
  error: function(jqXHR, textStatus, errorThrown) {
    if (window.inspect) {
      inspect([jqXHR.status, jqXHR.responseText]);
    }
    else {
      console.log(jqXHR.status + "\n" + jqXHR.responseText);
    }
  }
});

// Operation: get.
console.clear();
jQuery.ajax({
  url: '/service/restmini-example/example/thingie/2',
  type: 'GET',
  data: null,
  dataType: 'json',
  cache: false,
  success: function(oResp, textStatus, jqXHR) {
    if (window.inspect) {
      inspect([jqXHR.status, oResp]);
    }
    else {
      console.log(jqXHR.status + "\n" + oResp);
    }
  },
  error: function(jqXHR, textStatus, errorThrown) {
    if (window.inspect) {
      inspect([jqXHR.status, jqXHR.responseText]);
    }
    else {
      console.log(jqXHR.status + "\n" + jqXHR.responseText);
    }
  }
});


// (HEAD) Operation: index.
console.clear();
jQuery.ajax({
  url: '/service/restmini-example/example/thingie',
  type: 'HEAD',
  data: null,
  dataType: 'json',
  cache: false,
  success: function(oResp, textStatus, jqXHR) {
    if (window.inspect) {
      inspect([jqXHR.status, oResp]);
    }
    else {
      console.log(jqXHR.status + "\n" + oResp);
    }
  },
  error: function(jqXHR, textStatus, errorThrown) {
    if (window.inspect) {
      inspect([jqXHR.status, jqXHR.responseText]);
    }
    else {
      console.log(jqXHR.status + "\n" + jqXHR.responseText);
    }
  }
});
