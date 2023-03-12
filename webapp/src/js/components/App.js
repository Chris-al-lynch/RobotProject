
import React, { useEffect, useState } from 'react'
import { Outlet, useNavigate } from 'react-router-dom';
import MainButtonPanel from './utilities/MainButtonPanel';
import '../../css/App.css';

/***************************************************************************
 * WARNING TO ALL REACT DEVELOPERS:
 * 
 * After struggling with the router code and failing to get navigate
 * to work, I finally looked at the documentation for the react Component
 * class.  There it says don't use the component class.  Use functions instead.
 * All of the tutorials I looked at and the default code installed by react
 * all use the Component class.  This is very confusing/annoying for people
 * new to React.  The navigate function will not work in the Component class.
 * I'm assuming this is for React V6.  I see examples with history using
 * this Component class.  So, I am changing everything to use functions now.
 ***************************************************************************/

/**
 * The main application.  This is the entry into the project.  It contains
 * a button panel which has 2 buttons.
 */
export default function App()
{
   /* This code is used to load the home page on initial load only. */
   const navigate = useNavigate();
   const [startup, setStartup] = useState( true )

   useEffect( () => {
      if( startup )
      {
         navigate( '/home' );
         setStartup( false );
      }
   }, [startup]);

   return(
           <>
           <MainButtonPanel />
           <div>
             <h1 className="App-header">Robot Project</h1>
           </div>
           <div>
             <Outlet />
           </div>
           </>
         );
}
