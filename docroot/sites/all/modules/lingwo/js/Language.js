
/*
 * The generic framework for making/using a language definition.
 */

// Language
define('lingwo/Language', ['lingwo/util/extendPrototype'],
  function (extendPrototype) {
    // we string everything off of the Language constructor function.
    var Language = function () {
      // defer the actual setup to function defined later.
      _LanguageConstructor(this);
    };

    // stores all the language definitions
    Language.languages = {};

    // returns a function which can be called with a language object,
    // to create a constructor for the object specific to this language object.
    var makeClassMaker = function (actualCons, props) {
      return function (lang) {
        var cons = function () {
          actualCons.apply(this, arguments);
        };
        for (var name in props) {
          cons.prototype[name] = props[name];
        }
        cons.prototype.lang = lang;

        // IE hacks
        if (props['toString'] != Object.prototype.toString) {
          cons.prototype.toString = props.toString;
        }
        if (props['valueOf'] != Object.prototype.toString) {
          cons.prototype.valueOf = props.valueOf;
        }

        // used to extend the class
        cons.extend = function (obj) {
          for (var name in obj) {
            this.prototype[name] = obj[name];
          }
        };

        return cons;
      };
    };

    // A helper for making Array.splice() a little easier to work with.
    var arrayReplace = function (arr, i, len, items) {
      arr.splice.apply(arr, [i, len].concat(items));
    };

    var SubWord = function (word, start, len) {
      this.word = word;
      this.wordcls = word.lang.Word;
      this.start = (typeof start == 'undefined') ? -1 : start;
      this.len = len || 0;
    };
    extendPrototype(SubWord, {
      'drop': function () {
        if (this.start == -1) {
          return false;
        }

        var newWord = this.word.clone();
        newWord.letters.splice(this.start, this.len);
        return newWord;
      },
      'replace': function (text) {
        if (this.start == -1) {
          return false;
        }

        var rWord = this.word.lang.parseWord(text);
        var newWord = this.word.clone();
        arrayReplace(newWord.letters, this.start, this.len, rWord.letters);
        return newWord;
      },
      'append': function (text) {
        if (this.start == -1) {
          return false;
        }

        var rWord = this.word.lang.parseWord(text);
        var newWord = this.word.clone();
        arrayReplace(newWord.letters, this.start + this.len, 0, rWord.letters);
        return newWord;
      },
      'result': function (value) {
        if (this.start == -1) {
          return false;
        }
        return value;
      },
      'bool': function () {
        return this.start != -1;
      },
      'toWord': function () {
        return new this.wordcls(this.word.letters.slice(this.start, this.start + this.len));
      }
    });

    var makeWordClass = makeClassMaker(
      function (letters) {
        this.letters = letters || [];
      },
      {
        'clone': function () {
          return new this.lang.Word(this.letters.slice(0));
        },

        'append': function (text) {
          var newWord = this.clone();
          var rWord = this.lang.parseWord(text);
          arrayReplace(newWord.letters, newWord.letters.length, 0, rWord.letters);
          return newWord;
        },

        'concat': function (word) {
          return new this.lang.Word(this.letters.concat(word.letters));
        },

        'toString': function () {
          var chars = [];
          for (var i = 0; i < this.letters.length; i++) {
            var letter = this.letters[i];
            var letter_def = this.lang.alphabet[letter[0]];
            chars.push(letter_def.forms[letter[1]]);
          }
          return chars.join('');
        },

        'valueOf': function () { return this.toString(); },
        
        '_parseSpec': function (spec) {
          if (typeof spec == 'string') {
            return this.lang.parseWord(spec).letters;
          }
          if (typeof spec == 'object') {
            if (typeof spec.length == 'number') {
              // this is an Array, just return it
              return spec;
            }
            
            // pack into an array and return it
            return [spec];
          }

          throw ("Bad spec error");
        },

        '_compLetter': function (letter, letter_spec) {
          switch (letter_spec.spec) {
            case 'cls':
              return this.lang.letterHasClass(letter[0], letter_spec.value);
            default:
              // it is a letter!
              return letter[0] == letter_spec[0];
          }
        },

        'ending': function () {
          if (arguments.length == 1 && typeof arguments[0] == 'number') {
            // A length-based ending spec
            var len = arguments[0];
            if (len <= this.letters.length) {
              return new SubWord(this, this.letters.length - len, len);
            }
          }
          else {
            checki:
            for (var i = 0; i < arguments.length; i++) {
              var testSpec = this._parseSpec(arguments[i]);
              if (testSpec.length > this.letters.length) {
                continue checki;
              }

              for (var e = 0; e < testSpec.length; e++) {
                if (!this._compLetter(this.letters[this.letters.length-e-1], testSpec[testSpec.length-e-1])) {
                  continue checki;
                }
              }

              return new SubWord(this, this.letters.length - testSpec.length, testSpec.length);
            }
          }

          return new SubWord(this);
        },

        'hasEnding': function () {
          return this.ending.apply(this, arguments).bool();
        },

        'endingOrFalse': function () {
          var ending = this.ending.apply(this, arguments);
          if (ending.bool()) {
            return ending;
          }
          return false;
        }
      }
    );

    // the actual constructor function was defined above, but here we do
    // the real work.
    function _LanguageConstructor(lang) {
      lang.Word = makeWordClass(lang);
      lang.fields = {};
    };
    extendPrototype(Language, {
      alphabet: null,

      // Returns true if a letter in the alphabet has a specific class
      letterHasClass: function (name, cls) {
        var letterDef = this.alphabet[name];
        for (var i = 0; i < letterDef.classes.length; i++) {
          if (letterDef.classes[i] == cls) {
            return true;
          }
        }
        return false;
      },


      // creates a Word's "letter" from a string.
      //
      // A letter in this implementation is a 2 item array: [letter name, form name]
      //
      parseLetter: function (s) {
        if (!this.alphabet) {
          throw ("Cannot parseLetter() without a defined alphabet");
        }

        var name, form_name;
        for (name in this.alphabet)
        {
          var letter_def = this.alphabet[name];
          for (form_name in letter_def.forms)
          {
            if (letter_def.forms[form_name] == s) {
              return [name, form_name];
            }
          }
        }

        return null;
      },

      // take a string and get word out of it.
      parseWord: function (s) {
        // This function works by trying the whole text, and progressively
        // moving the start index forward, until a letter is found, then ignoring
        // the chunk that was found.  So, for example, if we had the word "mila"
        // we would try:
        //
        // 1. mila (no letter)
        // 2. ila  (no letter)
        // 3. la   (no letter)
        // 4. a  (found "a")
        // 5. mil  (no letter)
        // 6. il   (no letter)
        // 7. l  (found "l")
        // 8. mi   (... and so on)
        //
        // It is done this way to try and detect diagraphs in left-to-right
        // languages.  This will distinguish both "ch" from "h" and letters with
        // trailing accents like "o" from "o'".  I imagine, but havent tested, that
        // this would have to be reversed for right-to-left languages.

        if (!this.alphabet) {
          throw ("Cannot parseWord() without a defined alphabet");
        }

        var index = 0;
        var end   = s.length;

        var letters = [ ];
        while (end > 0)
        {
          index = 0;

          while (index <= end)
          {
            var test = s.substring(index, end);
            var res  = this.parseLetter(test);

            if (res !== null)
            {
              letters.push(res);
              end -= test.length;
              break;
            }

            index ++;
          }
          
          if (index > end)
          {
            // Nothing was found.
            /*
            if (console && console.debug) {
              console.debug("nothing was found");
            }
            */
            return null;
          }
        }

        // reverse the array
        letters.reverse();

        // build the word
        return new this.Word(letters);
      }

      // Makes a word into a string
      // TODO: This is copied from old code, modernize it!  This should really be put into the
      // constructor for Word.  We should accept an array of tuples, or possibly tuplize arguments
      // as shown below.
      /*
      createWord: function (letter_names, form) {
      //unparseWord: function (letter_names, form) {
        // Like the inverse of parse_word.  This takes a list of letter names and produces a 
        // word object.  The letter names can be a two item array with a specific form, or 
        // just a string that uses the form given.
        //
        //  For example:
        //
        // var word = lang.create_word([ 'b', 'y', 'c\'' ], 'lower');
        // var word = lang.create_word([ 'h', 'o', 'r', 'o', 'sh', ['o', 'lower.accent'] ], 'lower');
        //
        // Omitting the form will use the LetterDefs default form.
        //
        var word = new Lingwo.Word(this);

        for( var i = 0; i < letter_names.length; i++ )
        {
          var item = letter_names[i];
          if ( dojo.lang.isString(item) )
          {
            word.append_letter(item, form);
          }
          else
          {
            word.append_letter(item[0], item[1]);
          }
        }

        return word;
      }
      */
    });

    // Minor utility function
    Language.cls = function (cls) {
      return {'spec': 'cls', 'value': cls};
    };

    // the external function used to create new language definitions.
    Language.defineLanguage = function (name) {
      var lang = new Language();
      lang.name = name;
      Language.languages[name] = lang;
      return lang;
    };

    // function used to generate simple alphabets
    Language.generateAlphabet = function (single_letters, diagraphs, class_func) {
      class_func = class_func || function () { return []; };
      var alphabet = {}, i;
      var makeLetter = function (letter) {
        alphabet[letter] = {
          'classes': class_func(letter),
          'default_form': 'lower',
          'forms': {
            'lower': letter,
            'upper': letter.substr(0,1).toUpperCase() + letter.substr(1)
          }
        };
      };
      for (i = 0; i < single_letters.length; i++) {
        makeLetter(single_letters.substr(i,1));
      }
      for (i = 0; i < diagraphs.length; i++) {
        makeLetter(diagraphs[i]);
      }
      return alphabet;
    };

    return Language;
  }
);

