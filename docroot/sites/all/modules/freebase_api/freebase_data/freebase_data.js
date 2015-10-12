/**
 * @file Attach autocomplete behaviors to node forms.
 *
 *
 * When Freebase Suggest is attached to the title of a node
 * and a suggestion comes back,
 * Offer to fill in any empty fields with retrieved data.
 *
 * By naming the content type and the fields in a way
 * consistent with Freebase ids, we can deduce what data goes where.
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 */
 
 
 
/**
 * We've used freebase suggest to fill in the title, 
 * but now we actually need to record and use the machine name.
 * This behaviour reads the AJAX-retrieved key 
 * and triggers the next step.
 */
Drupal.behaviors.freebase_data_found_suggestion = function() {
  // suggest.js kindly fires an event when a suggestion was selected.
  // we can bind to listen to that
  $('#edit-title').bind('fb-select' , function() {
     console.log('fb-select was triggered, making info request'); 
    // fb suggest will have added a 'data' field to the suggested input element we can now inspect.
    var type_id = '';
    var type_element_data = $(this).data('data.suggest');
    var element = $(this);
    
    if (typeof(type_element_data) != 'undefined' && typeof(type_element_data.id) != 'undefined') {
      // the data should have a freebase id, and that's what we want!!
      topic_id = type_element_data.id

      // To find the relevant types to ask about, (the query needs to know)
      // We need background mapping information that we have put into the Drupal settings
      var content_type = Drupal.settings.freebase_data.content_type
      var topic_types = Drupal.settings.freebase_data.content_types[content_type]
      if (!topic_types) {
        console.log('Trouble. I don\'t know what type of topic I\'m asking for. I\'m suppose to have some clues from Drupal telling me what a content-type:"'+ content_type +'" is in Freebase.'); 
        return null;
      }

      console.log('Asking freebase for more detail about' + topic_id +' (Drupal:'+ content_type +', Freebase:'+ topic_types.join(',') +')'); 
      Drupal.freebase.load_info_about_topic(topic_id, topic_types);
    }
  })
}

/**
 * Create the 'freebase' connection object that we use to communicate with the service
 */
Drupal.behaviors.freebase_data = function (context) {
  Drupal.freebase = new freebase_connection();
}

/**
 * A freebase 'object' to handle the communications
 *
 * Needs to be more complex that we would hope due to cross-domain issues?
 */
function freebase_connection() {
  // Base url for autocomplete service
  this.service_url = "http://www.freebase.com";

  // Service_url + service_path = url to autocomplete service
  this.suggest_path = "/private/suggest";
  this.search_path = "/api/service/search";
  this.query_path = "/api/service/mqlread";
  // Note the content type we are looking for
  this.content_type = Drupal.settings.freebase_data.content_type;
  // Retrieve the mapping detail we will use later. Was stored in the js settings
  // mapping (no s) is just the subset of mapping*s* that apply to this content type
  this.mapping = Drupal.settings.freebase_data.mappings[this.content_type];
  
  this.retrieved_data = {};
}

freebase_connection.prototype.show_process_message = function(message) {
  if ($('#freebase-process').length == 0) {
    $('#freebase-message-box').append('<div id="freebase-process"></div>');
  }
  $('#freebase-process').html(message);
}

/**
 * Initiate the remote request for data we can put into the node edit form fields
 */
freebase_connection.prototype.load_info_about_topic = function(topic_id, topic_types) {

  // http://www.freebase.com/docs/web_services/mqlread
  // query={"query":[{"id":"/topic/en/edinburgh","key":[{"namespace":"/wikipedia/en_id","value":null}]}]}
  // mqlread nests a query inside a query (a query envelope)

// I want to fetch just everything
// But to get anything useful, need to probe deeper
//var query = [{
//  "/type/object/id": ""+topic_id,
//  "*":    [{}],
//}];
// That will not return any property specific to a type unless I name
// the type beforehand!
// And although I can tell it to get results from a thing of more than one type,
// 'type|=':  ['/government/politician', '/people/person'],
// that doesn't then return the per-type properties either.

// Which seems to mean we need to know the question before we ask it.
// There is no 'get everything you know'

// So, combine several requests, each based on the type (s) 
// we know we are looking for.

  if (! topic_types || topic_types.length == 0) {
    alert('Cannot get much data about '+ topic_id +' because we don\'t know what type of a thing it is');
    return NULL;
  }


// We ask for an array of objects for each possible attribute.
// An array, because many attributes are (rightly or wrongly) multiple
// An object (instead of just a string), because complex objects or large
// text results return IDs instead.
// When looking at the results, we will have to check for the type of 
// data object that was returned in each case.
// http://www.freebase.com/docs/mql/ch02.html#mqltypes
//
// Retrieving just results
//      "*":    [],
// did not return any usable description of complex 'compound types'
// that would have gone there.

  var queries = {};
  for (typeid in topic_types) {
    queries['q' + typeid] = {'query' : {
      "/type/object/id": "" + topic_id,
      "*":    [{}],
      'type': topic_types[typeid],
    }};
  }
  
  /*  EG:
  queries.q1 = {'query' : {
    "/type/object/id": ""+topic_id,
    "*":    [],
    'type': '/people/person'
  }};
  queries.q2 = {'query' : {
    "/type/object/id": ""+topic_id,
    "*":    [],
    'type': '/government/politician'
  }};
  */

  console.log(queries);

  // There is no stringify in jquery??
  // The included suggest.js includes json2 which has stringify!

  var queries_string = JSON.stringify(queries);
  console.log('Make a request for ' + queries_string);
  var jsonUrl = this.service_url + this.query_path;
  var jsonArgs = {
    'queries': queries_string , 
    'indent': '1',
    'format': 'json'
  }
  // Make storage space for the results
  this.retrieved_data[topic_id] = {};

  // Display the interim fetch results on the page  
  $("#freebase-message-box").remove();
  $('#edit-title-wrapper').after("<div id='freebase-message-box'></div>");
  this.show_process_message('Querying Freebase for data');
  this.recent_query = queries_string;

  // This nested function is a small work-around to ensure 
  // the handle to the current object gets passed through later.
  fb = this;
  this.getJSONp(jsonUrl, jsonArgs, function(json, status, res){fb.info_about_topic_loaded(json, status, res)} );
}



/**
 * Utility for creating queries to Freebase
 *
 * The BIG thing about talking to freebase is we must set
 * dataType: 'jsonp',
 * This means we cannot use jquery getJSON, and instead have to build our own 
 * ajax object
 */
freebase_connection.prototype.getJSONp = function(jsonUrl, jsonArgs, successfunc) {
  console.log('getJSONp from '+ jsonUrl);
  // Create the request
  $.ajax({
    url: jsonUrl,
    data: jsonArgs,
    success: successfunc,
    error: function(xhp, textStatus, errorThrown) {
      arh = xhp.getAllResponseHeaders();
      console.log(xhp);
      console.log(textStatus);
      console.log(errorThrown);
      alert(errorThrown);
    },
    dataType: 'jsonp',
  });
}


/**
 * Triggered when a request for data is returned
 * 
 * Returned data is remembered in freebase.retrieved_data.{topic_id}.{topic_type}
 
 * @see load_info_about_topic()
 */
freebase_connection.prototype.info_about_topic_loaded = function(json, status, res) {
  console.log('Retrieved info about topic!');

console.log(json);
console.log(status);
console.log(res);
console.log(this);

  freebase = Drupal.freebase;
  if (json.code != "/api/status/ok") {
    // Something failed, check for messages
    var message = json.messages.message
    alert("There was an error with loading the data definition : " + message);
    return false;
  }

  // Update the interim fetch results on the page
  this.show_process_message('Ready to absorb data');

  // I've made multiple queries, (or maybe just one) so they come
  // packed in an envelope.
  for (qid in json) {
    envelope = json[qid];
    console.log(qid);
    if (result = envelope.result) {
      console.log(result);
      //alert(result.id + " is a " + result.type)
      
      var topic_id = result['/type/object/id'];
      var type_id = result.type;
      // Store it for later if needed
      if (! freebase.retrieved_data[topic_id]) {
        freebase.retrieved_data[topic_id] = {};
      }
      freebase.retrieved_data[topic_id][type_id] = result;
      // retrieved data is an array, indexed by short freebase ID.
      // eg 'hometown'
      // We want long ID
      // '/location/hometown'
      
      // Display the results, and create some checkboxes 
      // to display the returned data status and let the user choose to absorb it.
      flattened_type = type_id.replace(/\//g, '_')
      flattened_type = ltrim(flattened_type, '_');

      var message = "We retrieved information from Freebase about"
      + "<em>" + result['/type/object/id'] + "</em>"
      + " as a "
      + "<strong>" + type_id + "</strong>"
      + " which we know as <strong>" + this.content_type + "</strong>"
      $('#freebase-message-box').append('<p id="about-'+ flattened_type +'">' + message + '</p>');

      
      // For each data field returned, find a place to put it on the current form
      for (datafield_id in result) {
        // There may be more than one result in this found_data.
        if (result[datafield_id].length) {
          this.find_field_to_store_data_in(datafield_id, topic_id, type_id, result[datafield_id]);
        }
        
      } // each data field in the result
    } // there was a result
  }
  // Processed all 'typed' results

  // At this point we know the names of all the target fields.

  // Add a button that will import this found data,
  $("#freebase-message-box").append('<input type="button" id="absorb-button" value="Absorb" />');
  $("#absorb-button").click(function(event){
    // Start the import
    Drupal.freebase.import_data(topic_id);
  });
    
  // freebase_message_box should now show all the fields we MAY be able to absorb into
  // Select the ones we SHOULD do - the ones that are currently empty
  $('#freebase-message-box .absorb-toggle').each( function () {
    var target_field = $(this).val();
    if ($("*[name='" + target_field + "[0][value]']").val() != '') {
      // The value of the target field we want to put data in already has some content
      $(this).attr('checked', 0)
    }
    else {
      $(this).attr('checked', 'checked');
    }
    console.log('Current value of ' + target_field + ' is ' + $("*[name='" + target_field +"']").val() );
  });
  
}

/**
 * See if the mapping detail - about the content type schema 
 * that was added as structured data in the settings in the header 
 * can tell us where this retrieved data should be put in the form.
 *
 * If we find a field on the page with the right name, create a checkbox
 * that will trigger the data import for that field
 */
freebase_connection.prototype.find_field_to_store_data_in = function(datafield_id, topic_id, type_id, found_data) {
        console.log("Looking for a CCK field to store " + datafield_id);

        var target_fields = this.mapping.reverse_mapping[datafield_id];
        // Look for a form field with this name
        if (! target_fields || target_fields.length == 0) {
          console.log("No known target fields to put '" + datafield_id + "' into");
          return;
        }
        for (possible_field in target_fields) {
          // mapping returned an array. So check each
          var field_name = target_fields[possible_field];
          var field_id = field_name.replace(/_/g, '-');
          console.log("Looking for a CCK field called " + field_name + " to match the freebase input called " + datafield_id);

          // CCK names it like name="people_person_place_of_birth[0][value]"
          var cck_form_field_name = field_name + '[0][value]';
          if ($("*[name='" + cck_form_field_name + "']").length) {

            short_data = 'data';
            if (typeof(found_data[0]) == 'string') {
              short_data = found_data[0].substring(0,20);
              if (found_data[0].length > 20) {
                short_data += "..."
              }
            }
  
            var toggle = '<input type="checkbox" class="absorb-toggle" name="absorb['+ topic_id +']['+ type_id +']['+ datafield_id +']" id="absorb-'+ field_id +'" value="'+ field_name +'">';
            toggle += '<label for="absorb-'+ field_id +'">Absorb "'+ short_data +'" into '+ field_id +'?</label>';
            $("#freebase-message-box #about-"+ flattened_type).append('<div class="insert-data-toggle">'+ toggle +'</div>');
  
            // Save some data on this element so we can remember what it is talking about.
            $('#absorb-'+ field_id)
              .data('topic_id', topic_id)
              .data('type_id', type_id)
              .data('datafield_id', datafield_id);

            var hovertext = '';
            console.log(typeof(found_data));
            if (typeof(found_data[0]) == 'string') {
              hovertext = found_data[0].substring(0,20)
            }
            $("#freebase-message-box  label[for='"+ 'absorb-'+ field_id +"']").attr('title', hovertext);
          } // found a form field
          else {
            console.log("No form field called " + cck_form_field_name + " on this page");
          }
        } // each possible field it could go into
}

/**
 * The 'Absorb' buttone was clicked.
 *
 * Use the remembered data arrays, and the user-chosen options to copy content 
 * from the retrieved data into the form fields.
 */
freebase_connection.prototype.import_data = function(topic_id) {
  var retrieved_data = this.retrieved_data;
  this.show_process_message('Importing selected data for ' +topic_id );

  // Get the user choices.
  $('#freebase-message-box .absorb-toggle').each(function(){
    // The val() of this element names the target element we need to write to
    // the @name of this element names the source data in our retrieved_data array 
    if ($(this).attr('checked')) {
      // Need to import this one.
      var target_field_name = $(this).val();
      var this_checkbox_id = $(this).attr('id');
      // Info about what data to put there was stored as data on the element
      var topic_id = $(this).data('topic_id');
      var type_id = $(this).data('type_id');
      var datafield_id = $(this).data('datafield_id');
      var data_to_put_there = retrieved_data[topic_id][type_id][datafield_id]
      console.log(target_field_name);
      console.log(data_to_put_there);
      // the data is an array, may be multiple, CCK supports that.
      // Trust that its numeric indexes match the numeric indexes CCK uses
      for (delta in data_to_put_there){
        // check that the form field(s) we are looking for is there.
        var target_element_name = target_field_name + '[' + delta +'][value]'

        // If it's multiple, and cck hasn't given us enough fields, get it to add another!
        if (! $("*[name='" + target_element_name +"']").length) {
          // click the add more button
          console.log("No room for "+ target_field_name +"["+ delta +"]");
          // This doesn't work, and it's asyncronous, which we can't cope with
          // TODO - what?
          //$("*[name='" + target_field_name +"_add_more']").click();
        }

        // Set it!
        console.log(data_to_put_there[delta]);
        // the data is itself an object, find its string value if we can.
        // Most data is a string, but complex references are not
        // so far, just deal with string
        if (data_to_put_there[delta].name) {
          data_value = data_to_put_there[delta].name;
        }
        else if (data_to_put_there[delta].id) {
          data_value = data_to_put_there[delta].id;
        }
        else {
          data_value = data_to_put_there[delta];
        }
        
        console.log("Setting "+ target_element_name + " to "+ data_value);
        $("*[name='" + target_element_name +"']").val(data_value);
      }
    }
  });
}

///////////////////////////////////////

function ltrim(str, chars) {
  chars = chars || "\\s";
  return str.replace(new RegExp("^[" + chars + "]+", "g"), "");
}
