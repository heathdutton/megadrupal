This little module is an extension to the rules included in commerce 
to help you setup shipping costs.
It will allow you to add conditions selecting the vocabularies that you need 
and the terms you want to base your condition on.

The module will check all the products in the basket(commerce order) to see if 
there are matches with the conditions you have set.

This module needs commerce line item(included in the commerce module) 
and taxonomy to run.

How to use:
Go to admin -> Store -> config -> shipping
-> choose the shipping rate you want to add the conditions based on 
   taxonomy too.
-> Configure component
-> Add condition
-> check from the selectbox "Order contains a product with a 
   certain term(taxonomy)"
-> wait for the form to load and choose a "Term Reference Field" 
   (taxonomy vocabulary)
-> Next choose a taxonomy term you want to filter on.

if you want you can also base a amount on it to make your
condition more specific.

After filling it out you can save the condition to get it workin.
