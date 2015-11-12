(function ($) {

  Drupal.netforum = Drupal.netforum || {};
  
  Drupal.behaviors.netforum = {
    attach: function(context) {
        //first step, bind to the change event for our function drop down list
        $('#edit-netforum-request').change(function(){
            return Drupal.netforum.updateParameterForm();
          });
          //second step, swap out the four default parameters that drupal renders
          //and swap in the parameters for whatever is selected

          Drupal.netforum.updateParameterForm();
    }
  };
  
  Drupal.netforum.getParamsForSelectedFunction = function () {
    return xweb_functions[Drupal.netforum.getSelectedFunction()];
  };

  Drupal.netforum.getSelectedFunction = function () {
    return $('#edit-netforum-request').val();
  };

  //this is just a wrapper to animate the form changes
  Drupal.netforum.updateParameterForm = function () {

    $('#parameters').hide("fast");
    if (Drupal.netforum.getParamsForSelectedFunction() == "parameters") {
      $.getJSON(document.location.pathname + "/params_for/" + Drupal.netforum.getSelectedFunction(), {}, function(json){
        xweb_functions[Drupal.netforum.getSelectedFunction()] = json;
        Drupal.netforum.updateParameters(Drupal.netforum.getParamsForSelectedFunction());
        $('#parameters').show("normal");
      });
    }
    else {
      Drupal.netforum.updateParameters(Drupal.netforum.getParamsForSelectedFunction());
      $('#parameters').show("normal");
    }

  };


  //This function looks at the selected function, references it with an array placed in the page
  //header by the drupal render, and adjusts the fields displayed.  They are all named
  //netforum_param_1, netforum_param_2, etc
  Drupal.netforum.updateParameters = function (parameter_list) {
    
    //the xweb_functions variable is included in the head of the page by the module.
    //it is an array with the function names and the parameters listed

    var paramsDiv = $('#parameters div.fieldset-wrapper');
    paramsDiv.empty();
    if (parameter_list.length == 0) {
      paramsDiv.html("<i>no parameters</i>");
    }
    else {
      Drupal.netforum.addParamsInto(paramsDiv[0], parameter_list);    
    }
  };

  Drupal.netforum.addParamsInto = function (paramsDiv, parameter_list) {
    if (typeof(Drupal.netforum.addParamsInto.paramId) == "undefined") {
      Drupal.netforum.addParamsInto.paramId = 0;
    }
  
    if (typeof(Drupal.netforum.addParamsInto.objStack) == "undefined") {
      Drupal.netforum.addParamsInto.objStack = [];
    }
  
    jQuery.each(parameter_list, function(key, val) {
      if (typeof(val) == "string") {
        Drupal.netforum.addParamsInto.paramId += 1;
      	paramsDiv.appendChild(Drupal.netforum.buildInputDiv(Drupal.netforum.addParamsInto.paramId, val, Drupal.netforum.addParamsInto.objStack));
      }
      else if (typeof(val) == "object") {
        Drupal.netforum.addParamsInto.objStack.push(key);
      	var legend = document.createElement('legend');

        // Drupal 7 collapse requires extra elements. See http://drupal.org/node/1424350#comment-5548762
        var legendSpan = document.createElement('span');
        legendSpan.setAttribute('class', 'fieldset-legend');
        legendSpan.innerHTML = key;
        legend.appendChild(legendSpan);
    	
        var newFieldset = document.createElement('fieldset');
				// TODO: Add the 'collapsed' class
        // For now this makes the form disappear completely,
        // since the click-to-expand links aren't added properly
      	newFieldset.setAttribute('class', 'collapsible');
      	newFieldset.setAttribute('title', key);
      	newFieldset.setAttribute('id', 'fieldset-'+Drupal.netforum.addParamsInto.paramId);
      	newFieldset.appendChild(legend);

        var wrapper = document.createElement('div');
        wrapper.setAttribute('class', 'fieldset-wrapper');

      	Drupal.netforum.addParamsInto(wrapper, val);
    	
      	newFieldset.appendChild(wrapper);
      	paramsDiv.appendChild(newFieldset);
        // This makes our fieldset disappear completely
        // Drupal.behaviors.collapse();
        Drupal.netforum.addParamsInto.objStack.pop();
      }
    });
  };

  Drupal.netforum.buildInputDiv = function (pnum, pname, stack) {
    var prefix = "";
    if (stack.length > 0) {
      prefix = "[" + stack.join("][") + "]"; 
    }
  
    var newDiv = document.createElement('div');
  	newDiv.setAttribute('id', 'netforum_custom_'+pnum);
  	newDiv.setAttribute('class', 'form-item');

  	var newLabel = document.createElement('label');
  	newLabel.setAttribute('for', 'edit-netforum-param-'+pnum);
  	newLabel.innerHTML = pname + ":";
  	newDiv.appendChild(newLabel);

  	var newInput = document.createElement('input');
  	newInput.setAttribute('type', 'text');
  	newInput.setAttribute('maxlength', '2000');
  	newInput.setAttribute('name', 'netforum_params'+prefix+"["+pname+"]");
  	newInput.setAttribute('id', 'edit-netforum-param-'+pnum);
  	newInput.setAttribute('size', 60);
  	newInput.setAttribute('class', 'form-text');
	
    // When we are redisplaying a form, this var will be populated.  We work through
    // the stack and walk through the vars to set the defaults
  	if (typeof(form_defaults) != "undefined" && form_defaults[Drupal.netforum.getSelectedFunction()]) {
  	  var tmpStack = stack,
  	      tmpStackLen = stack.length,
  	      defaults = form_defaults[Drupal.netforum.getSelectedFunction()];

  	  for (i=0;i<tmpStackLen;i++) {
  	    if (typeof(defaults[tmpStack[i]]) != "undefined") {
  	      defaults = defaults[tmpStack[i]];
  	    }
  	  }
  	  if (typeof(defaults[pname]) != "undefined") {
  	    newInput.value = defaults[pname]; 
  	  }
  	}
  	newDiv.appendChild(newInput);
  	return newDiv;
  };
  
  
})(jQuery);
