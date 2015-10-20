@TODO: add modal description

# CONTENTS OF THIS FILE
---
## INTRODUCTION

## PLANNED FEATURES

## INSTALLATION

## DEVELOPER USAGE

---

### Popover

There are some possibles to use the bootstrap popovers.

#### by Element API (recommend)
this is the recommend way because you get full drupal behaviors support and you dont need to load the library. You only need to add the #bootstrap_api_popover attribute as array() to an element. The keys in the array are the same like the bootstrap options.

**title** and **content** are **required!**

Example:

    <?php
    function HOOK_form_user_register_form_alter(&$form, &$form_state) {
        $form['account']['name']['#bootstrap_api_popover'] = array(
            'title' => t('Example popover title'),
            'content' => t('Yeah popovers are cool'),
            'trigger' => 'click',
        );
    }
    ?>
    
Options:
Use the same options as bootstrap additional there are special options:

'#use_description' => If set to TRUE it uses the element #description key as content. If set to "clean" it clean the #description after use.


<table class="table table-bordered table-striped">
          <thead>
           <tr>
             <th style="width: 100px;">Name</th>
             <th style="width: 100px;">type</th>
             <th style="width: 50px;">default</th>
             <th>description</th>
           </tr>
          </thead>
          <tbody>
           <tr>
             <td>animation</td>
             <td>boolean</td>
             <td>true</td>
             <td>apply a css fade transition to the tooltip</td>
           </tr>
           <tr>
             <td>html</td>
             <td>boolean</td>
             <td>false</td>
             <td>Insert html into the popover. If false, jquery's <code>text</code> method will be used to insert content into the dom. Use text if you're worried about XSS attacks.</td>
           </tr>
           <tr>
             <td>placement</td>
             <td>string|function</td>
             <td>'right'</td>
             <td>how to position the popover - top | bottom | left | right</td>
           </tr>
           <tr>
             <td>selector</td>
             <td>string</td>
             <td>false</td>
             <td>if a selector is provided, tooltip objects will be delegated to the specified targets</td>
           </tr>
           <tr>
             <td>trigger</td>
             <td>string</td>
             <td>'click'</td>
             <td>how popover is triggered - click | hover | focus | manual</td>
           </tr>
           <tr>
             <td>title</td>
             <td>string | function</td>
             <td>''</td>
             <td>default title value if `title` attribute isn't present</td>
           </tr>
           <tr>
             <td>content</td>
             <td>string | function</td>
             <td>''</td>
             <td>default content value if `data-content` attribute isn't present</td>
           </tr>
           <tr>
             <td>delay</td>
             <td>number | object</td>
             <td>0</td>
             <td>
              <p>delay showing and hiding the popover (ms) - does not apply to manual trigger type</p>
              <p>If a number is supplied, delay is applied to both hide/show</p>
              <p>Object structure is: <code>delay: { show: 500, hide: 100 }</code></p>
             </td>
           </tr>
          </tbody>
        </table>
        

#### by REL & DATA Attributes
You can use the bootstrap variant to bind an popover to an element.

    <a href="#" rel="popover" data-title="Example title" data-content="Yeah… popovers are cool!" />
    
but you need to add the library manually eg. in an hook_init

    <?php
    function HOOK_init() {
      drupal_add_library('bootstrap_api', 'bootstrap-api.popover');
    }
    ?>
    
**Notice:**
*I dont recommend this way, because it can have side effects with the drupal #ajax behaviors.*
