import OvalButton from "./OvalButton";

/**
 * Used to display and implement the oval configuration button.
 */
export default class ConfigButton extends OvalButton
{
   openConfig()
   {
      console.log( "OPENED CONFIGURATION" );
   }

   render()
   {
      return( <OvalButton value="Configure"
                          onClick={ this.openConfig } />  );
   }
}