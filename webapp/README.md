This component is the webapp functionality.  

Progress:

Docker container
Used react-bootstrap to set up the layout.  Have the menu buttons, but probably going to change these in the future.  I just liked the way they looked and wanted to keep them around for a little while.

Still working on the configuration page.

Config page:

Will ultimately read the database for all possible configurations and list them by name.  The user will be able to select an existing configuration or add a new one.

If an existing one is selected, a popup will occur with the configuration information and ask the user to verify that is the desired configuration.  If not, will go back to the selection form.  If so, will jump to the play form.

If the user selects to add a new one, the page will switch to the configuration form where they can enter the new configuration information.

During this exercise I learned that hooks must be called at the top level no matter what.  I attempted to create a fake database using state variables hidden from the rest of the code.  React doesn't work like normal code because it's purpose is to build HTML.  I also went through the effort to try this Redux.  It did not work any differently.  So, my next step is to move to the mysql component and get that working before continuing with the webapp component.
