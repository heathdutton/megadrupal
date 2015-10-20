(function(Popcorn) {
  Popcorn.plugin("classactivator", (function() {
    return {
      manifest : {
        about : {
          name : "classactivator",
          version : "1.0.0",
          author : "Kristoffer Wiklund",
          website : "http://www.websystem.se"
        },
        options : {
          start : {
            elem : "input",
            type : "text",
            label : "In"
          },
          end : {
            elem : "input",
            type : "text",
            label : "Out"
          },
          target : "id-selector",
          classname : "classname",
        }
      },
      _setup : function(options) {
        options._container = Popcorn.dom.find(options.target);
      },
      start : function(event, options) {
        options._container.className = options._container.className + " " + options.classname;
      },
      end : function(event, options) {
        var regex = new RegExp("\\b" + options.classname + "\\b");
        options._container.className = options._container.className.replace(regex, '').trim();

      },
    };
  })());
})(Popcorn);
