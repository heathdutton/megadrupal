/**
 * @file Form UI enhancement to update the Freebase ID once a type name has 
 * been selected.
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 */

/**
 * We use freebase suggest to fill in the human name of the type definition, 
 * but we actually need to record and use the machine name.
 * This reads the AJAX-retrieved key and puts in into the form where it can 
 * be submitted.
 */
Drupal.behaviors.freebase_cck_save_suggestion = function() {
  // Binding to 'change' seems to be premature, the data is not ready yet.
  // The suggest.js script binds to a lot of things, not sure where to catch the event.
  // 'blur' seems to work.
  $('.chosen-type').blur( function() {
    // fb suggest will have added a 'data' field to the suggested input element we can now inspect.
    var type_id = '';
    var type_element_data = $(this).data('data.suggest');
    console.log(type_element_data);
    if (typeof(type_element_data) != 'undefined' && typeof(type_element_data.id) != 'undefined') {
      // the data should have a freebase id, and that's what we want!!
      type_id = type_element_data.id
      $('.chosen-type-id').val(type_id);
    }
  })
}


Drupal.behaviors.freebase_schema = function (context) {
  Drupal.freebase = new freebase_connection();
  Drupal.freebase.form_element = $('#edit-service-wrapper .freebase-topic');

  // Set up actions on the form buttons to trigger AJAX searches
/*
  $('#edit-service-wrapper .freebase-topic .list-available-types').click(
    function (event) {
      // go get the list of available topic types from freebase
      //alert('start listing types now')
      freebase.list_available_types();
      //return false;
    }
  );
*/
/*
  $('#edit-service-wrapper .freebase-topic .load-type-definition').click(
    function (event) {
      // go get the list of available topic types from freebase
      alert('start listing types now')
      Drupal.freebase.load_type_definition();
      event.preventDefault();
      event.stopPropagation();
      return false;
    }
  )
*/
}

/**
 * A freebase 'object' to handle the communications
 *
 * Needs to be more complex that we would hope due to cross-domain issues?
 */
function freebase_connection() {
  // base url for autocomplete service
  this.service_url = "http://www.freebase.com";

  // service_url + service_path = url to autocomplete service
  this.suggest_path = "/private/suggest";
  this.search_path = "/api/service/search";
  this.query_path = "/api/service/mqlread";
}

freebase_connection.prototype.load_type_definition = function() {
  alert('making ajax call now')
  // Create the request
  // fb suggest will have added a 'data' field to the suggested input element we can now inspect.
  var type_element = $('.chosen-type', this.form_element) 
  if (typeof(type_element.data) != 'undefined') {
    this.type_element_data = type_element.data("data.suggest");
    // the data should have a freebase id, and that's what we want!!
    var type_id = this.type_element_data.id
  }

  var chosen_type_id_element = $('.chosen-type-id', this.form_element);
  if (! type_id) {
    // We may be able to skip this if the chosen-type-id was already defined and remembered
    if (chosen_type_id = chosen_type_id_element.val()) {
      // re-using the id entered manually or remembered
      type_id = chosen_type_id;
    }
    else {
      alert("Need to enter and select a valid type");
      return FALSE;
    }
  }
  chosen_type_id_element.val(type_id);
  
  alert(this.type_element_data.name + " is " + type_id);

  // http://www.freebase.com/docs/web_services/mqlread
  // query={"query":[{"id":"/topic/en/edinburgh","key":[{"namespace":"/wikipedia/en_id","value":null}]}]}
  // mqlread nests a query inside a query (a query envelope)

/*
To retrieve the names, types & ids of the expected properties of the 'book' type
here is a query to tell me about this thing

[{
  "/type/object/id": "/book/book",
  "/type/object/type": "/type/type",
  "/type/type/properties": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null
  }],
  "/freebase/type_hints/included_types" : []
}]

// http://www.freebase.com/docs/data/going_meta

// Tell me about this thing AND all the info about things it inherits from
// A book has properties like 'Genre' but is also a "Written work"
// And a "Written work" has properties like "Author"
// I'll want all sets
[{
  "/type/object/id": "/book/book",
  "/type/object/name": null,
  "/type/object/type": "/type/type",
  "/type/type/properties": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/property/reverse_property": null,
    "/type/property/unique": null,
  }],
  "/freebase/type_hints/included_types": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/type/properties": [{
      "/type/object/id": null,
      "/type/object/name": null,
      "/type/property/expected_type": null,
      "/type/property/reverse_property": null,
      "/type/property/unique": null,
    }]
  }]
}]
*/
var query = [{
  "/type/object/id": ""+type_id,
  "/type/object/type": "/type/type",
  "/type/type/properties": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/property/reverse_property": null,
    "/type/property/unique": null,
  }],
  "/freebase/type_hints/included_types": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/type/properties": [{
      "/type/object/id": null,
      "/type/object/name": null,
      "/type/property/expected_type": null,
      "/type/property/reverse_property": null,
      "/type/property/unique": null,
    }]
  }]
}]
  // There is no stringify in jquery??
  // The included suggest.js includes json2 which has stringify!
  query_string = JSON.stringify(query);

  var jsonUrl = this.service_url + this.query_path;
  var jsonArgs = {
    'query': '{"query":' + query_string +'}', 
    'indent': '1', 
    'format': 'json'
  }
  this.getJSONp(jsonUrl, jsonArgs, this.type_definition_loaded );
}


/**
 * The BIG thing about talking to freebase is we must set
 * dataType: 'jsonp',
 * This means we cannot use jquery getJSON and have to build our own ajax object
 */
freebase_connection.prototype.getJSONp = function(jsonUrl, jsonArgs, successfunc) {
  alert('getJSONp from '+ jsonUrl);
  // Create the request
  $.ajax({
    url: jsonUrl,
    data: jsonArgs,
    success: successfunc,
    error: function(xhp, textStatus, errorThrown) {
      arh = xhp.getAllResponseHeaders();
      alert(errorThrown);
    },
    dataType: 'jsonp',
  });
}


/**
 * Triggered when a request for a type definition is returned
 */
freebase_connection.prototype.type_definition_loaded = function(json, status, res) {
  alert('Retrieved definition of a specific type!');
  freebase = Drupal.freebase;
  if (! json.result) {
    // Something failed, check for messages
    var message = json.messages.message
    alert("There was an error with loading the type definition " + message);
    return false;
  }

  var resultset = json.result;
  
  // The json.result is an array containing one (lookup result) object 
  // containing a set of attributes, 
  // each of which is an array containing various values.
  /*
  "result": [
    {
      "creator": [
        {
          "id": "/user/metaweb",
          "name": "Freebase Staff",
          "type": [
            "/type/user"
          ]
        }
      ],
    }
  */
  var resultobj = resultset[0];
  if (!resultobj) {
    alert("No result :-(");
    return false;
  }
  $(".freebase-topic .type-definition").html("Loaded content scema from FreeBase");

  $(".freebase-topic .type-definition").append(freebase.dump_obj(resultset));

}

/**
 * Utility to dump a json big nested object into HTML list
 * debug
 */
freebase_connection.prototype.dump_obj = function(obj) {
  var type = typeof(obj);
  var output;
  switch(type) {
    case 'string' :
    case 'int' :
    case 'boolean' :
      return $("<span>" + obj + "</span>");
    case 'array' :
      output = $("<ol>");
      for (var obj_i in obj) {
        var li = $("<li>");
        li.append(this.dump_obj(obj[obj_i]))
        output.append(li);
      }
      return output;
    case 'object' :
      output = $("<ul>");
      for (var obj_i in obj) {
        var li = $("<li>")
          .append($("<span>")
            .append($("<label>").text(obj_i)).append(" : ")
          )
          .append(this.dump_obj(obj[obj_i]))
        output.append(li);
      }
      return output;
    default :
        alert("" + type + " " + obj);
  }
  return "";
}


///////////////////////////////////////


freebase_connection.prototype.list_available_types = function() {
  alert('making ajax call now')
  // Create the request
  var jsonUrl = this.service_url + this.search_path;
  // Return a list of all things that are type definitions
  var jsonArgs = {
    'type': "/freebase/type_profile", 
    'query': "", 
    'indent': '1', 
    'format': 'json'
  }
  this.getJSONp(jsonUrl, jsonArgs, this.types_loaded);
 
  // alert("jsonp is requested from "+jsonUrl);  
  
}

freebase_connection.prototype.types_loaded = function(json, status, res) {
  alert('Types Load was performed.');
  if (! json.result) {
    // Something failed, check for messages
    var message = json.messages.message
    alert(message);
    return false;
  }
  
  var results = json.result;
  var result_text = "Result was \"<strong>" + json.result + "\"";
  $(".freebase-topic .available-types").html(result_text);

  $.each(results, function(i, data) {
      li = $("<li>"); // .addClass(css.item);
      var label = $("<label>").text(data.name);
      data.name = label.text();
      li.append($("<div>").append(label));
      $(".freebase-topic .available-types").append(li);
    }
  )

}
