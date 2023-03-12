import React from 'react';
import ReactDOM from 'react-dom/client';
import { createBrowserRouter, RouterProvider } from 'react-router-dom'
import { ThemeProvider } from 'react-ui';
import { tokens, components } from 'react-ui/themes/dark';

import './css/index.css';

import App from './js/components/App';
import ConfigPage from './js/components/config/ConfigPage.js';
import ErrorPage from './js/components/ErrorPage';
import PlayPage from './js/components/play/PlayPage.js';


const router = createBrowserRouter( [ { path: "/",
                                        element: <App/>,
                                        errorElement: <ErrorPage />,
                                        children:
                                          [
                                            { path: "/config",
                                              element: <ConfigPage/>,
                                            },
                                            { path: "/play",
                                              element: <PlayPage/>,
                                            }
                                          ]
                                      }
                                    ]
                                  );

/**
 * This code simply invokes the App component which is the main component
 * in this project.
 */
const root = ReactDOM.createRoot( document.getElementById( 'root' ) );

/* TODO: StrictMode enables extra checks and debugging useful during development.
 *       Once finished, remove StrictMode
 */
root.render( <React.StrictMode>
               <ThemeProvider tokens={tokens} components={components}>
                  <RouterProvider router={router} />
               </ThemeProvider>
             </React.StrictMode> );
