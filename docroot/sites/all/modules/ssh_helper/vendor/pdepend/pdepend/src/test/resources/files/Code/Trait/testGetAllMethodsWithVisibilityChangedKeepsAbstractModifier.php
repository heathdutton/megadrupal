<?php
trait testGetAllMethodsWithVisibilityChangedKeepsAbstractModifier
{
    use testGetAllMethodsWithVisibilityChangedKeepsAbstractModifierUsedTraitOne {
        foo as protected;
    }
}

trait testGetAllMethodsWithVisibilityChangedKeepsAbstractModifierUsedTraitOne {
    public abstract function foo();
}
