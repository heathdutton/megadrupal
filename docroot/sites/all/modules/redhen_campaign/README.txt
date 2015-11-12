# RedHen Campaign

RedHen Campaign provides a framework for building viral fundraising content. It
is built on top of the RedHen CRM collection of modules, and relies on both
RedHen and RedHen Donation (which requires Drupal Commerce) to function.

RedHen Campaign creates two basic entity types, which can be configured under
Structure -> RedHen CRM.

## Entities

RedHen Campaign (redhen_campaign) is the core entity type. It offers a
fundraising campaign which can collect donations, and also serve as the parent
for RedHen Campaign Pages, allowing those sub-fundraisers to work towards the
same fundraising goals as the Campaign. The entity is fully fieldable, and has
both built-in fields (Goal, Donation Type) and the ability to be built up with
field content to match your needs.

RedHen Campaign Page (redhen_campaign_page) is an entity type for allowing site
visitors to build out their own sub-campaigns attached to RedHen Campaigns.
These can have their own start and end dates and goals, are fully fieldable, and
must be related to Campaigns. Campaign Pages can be of two basic types: Team or
Individual. Team Campaigns can have other Campaign Pages as Members, allowing
Teams to be formed around Campaigns and build fundraising teams as Individuals.

## Field Display Widgets

The "Smart Thermometer" display widget can be used with the Goal field to
easily display a Progress thermometer, including or excluding a text description
of the numbers.

For certain basic content fields on Campaign Pages, like text and image fields,
you will notice an option on the Display configuration Format called "Inherit
Campaign Value". This formatter can be configured by choosing Default,
Secondary, and Tertiary display options from "Campaign", "Team", and "Page".
If "Campaign" is selected as the Default, the display mode will look for an
instance of this field on the Page's parent campaign, and render the Campaign's
value if it finds one.

This allows you to enforce some common look and feel across a Campaign's various
member pages. It also allows the Campaign's value for a field to serve as a
default for newly created pages, allowing an individual to create a personal
fundraiser without inputing content into every field right away.

## Distribution

RedHen Campaign is a central part of the Drupal Distribution "RedHen Raiser".
RedHen Raiser includes a complete, customizable theme, various widgets for
showing off campaign contributors and progress, and is an excellent starting
point for building your own social fundraising site or exploring what can be
done with RedHen Campaign.
