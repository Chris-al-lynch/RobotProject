
import { createBrowserRouter } from 'react-router-dom'

import App from '../App';
import ConfigPage from '../config/ConfigPage.js';
import ConfigForm from '../config/ConfigForm.js';
import ErrorPage from '../errors/ErrorPage';
import HomePage from '../HomePage';
import PlayPage from '../play/PlayPage.js';

/**
 * Define all of the routes for the project.
 */
const router = createBrowserRouter( [ { path: "/",
                                        element: <App/>,
                                        errorElement: <ErrorPage />,
                                        children:
                                          [
                                            { path: "/home",
                                              element: <HomePage/>,
                                            },
                                            { path: "/configselect",
                                              element: <ConfigPage/>,
                                            },
                                            { path: "/configform",
                                              element: <ConfigForm/>,
                                            },
                                            { path: "/play",
                                              element: <PlayPage/>,
                                            }
                                          ]
                                      }
                                    ]
                                  );

export default router;