import React from 'react';
import ReactDOM from 'react-dom/client';
import 'bootstrap/dist/css/bootstrap.css';

import { RouterProvider } from 'react-router-dom'

import './css/index.css';

import router from './app/routes/Routes';

/**
 * This code simply invokes the App component which is the main component
 * in this project.
 */
const root = ReactDOM.createRoot( document.getElementById( 'root' ) );

document.body.style = 'background: rgba( 33, 37, 41 )';

/* TODO: StrictMode enables extra checks and debugging useful during development.
 *       Once finished, remove StrictMode
 */
root.render( <React.StrictMode>
               <div className="bg-dark">
                <RouterProvider router={router} />
               </div>
             </React.StrictMode> );