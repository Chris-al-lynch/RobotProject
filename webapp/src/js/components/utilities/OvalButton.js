import { useNavigate } from 'react-router-dom';
import '../../../css/OvalButton.css';

/**
 * Component for rendering a pretty oval button.
 * 
 * TODO: Figure out how to vary the color of the button programatically.
 */
export default function OvalButton( value, path )
{
   const navigate = useNavigate();

   function handleClick()
   {
      navigate( path );
      console.log( "Going to page " + path );

   }

   return(
        <button className="btn btn-oval" onClick={handleClick}>
           {value}
        </button> );
}