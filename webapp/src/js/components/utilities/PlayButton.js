import OvalButton from "./OvalButton";

/**
 * Used to display and implement the oval play button.
 */
export default class PlayButton extends OvalButton
{
   openPlay()
   {
      console.log( "OPENED PLAY" );
   }

   render()
   {
      return( <OvalButton value="Play"
                          onClick={ this.openPlay } />  );
   }
}