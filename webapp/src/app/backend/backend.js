import axios from 'axios'

export async function getAllConfigNames()
{
    const data = await getConfig();

    console.log( "getAllConfigNames(): data = " + JSON.stringify( data ) );
    
    const names = [];

    if( data != null )
    {
        for( let d = 0; d < data.length; d++ )
        {
            names.push( data[d].name );
        }
    }

    return names;
}

export async function deleteConfig( item )
{
    console.log( "Deleting item " + item )
    const response = await axios.delete( '/Robot/' + item )

    /* TODO: Handle errors */
    return response;
}

export async function getConfig()
{
    const response = await axios.get( '/Robot/' );

    // FIXME: handle response errors
    return response.data;
}

export function addNewConfig( data )
{
    console.log( "axios.post(): Data = " + JSON.stringify( data ) );
    const response = axios.post( '/Robot/', { name: data[0].value,
                                              robotIPAddress: data[1].value,
                                              robotPort: data[2].value,
                                              description: data[3].value} )
                          .then( ( resp ) => { return resp } );
    // FIXME: handle response errors
    return response
}

export function updateConfig( data )
{
    console.log( "axios.put(): Data = " + data );
    const response = axios.put( '/Robot/', { name: data[0].value,
                                             robotIPAddress: data[1].value,
                                             robotPort: data[2].value,
                                             description: data[3].value} )
                          .then( (resp) => { return resp } );
    /*
                                           { name: data.name,
                                             robotIPAddress: data.address,
                                             robotPort: data.port,
                                             description: data.description} )
                                             */
    // FIXME: handle response errors
    return response
}