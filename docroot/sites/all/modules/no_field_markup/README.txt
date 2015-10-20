The no_field_markup module is a small module which removes the <div> elements
around a Field on output. In addition, it kicks away the label too.
You can control this possibility by each field.

Have a look of this example:

<div class="field field-name-field-firstname field-type-text field-label-above">
	<div class="field-label">Firstname:</div>
	<div class="field-items">
		<div class="field-item even">Dan</div>
	</div>
</div>

is now:

Dan

----

You can control this possibility for each field in the field configuration,
next to the "is required" checkbox.


thank to this module, you have now a smarter domtree and
as a "hardcore" themer more flexibility.
