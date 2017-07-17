//
//  Shader.fsh
//  openglEs
//
//  Created by Marius-Mihai Sincovici on 11/12/15.
//  Copyright © 2015 marius. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
