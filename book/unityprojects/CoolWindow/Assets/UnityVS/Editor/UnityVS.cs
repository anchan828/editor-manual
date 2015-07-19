using UnityEngine;
using System.Collections;
using System;
using SyntaxTree.VisualStudio.Unity.Bridge;
using System.Xml.Linq;
using System.Linq;
using UnityEditor;

[InitializeOnLoad]
public class UnityVS
{

    static UnityVS()
    {
        ProjectFilesGenerator.SolutionFileGeneration += (string filename, string content) =>
        {
            var document = XDocument.Parse(content); 
             document.Root.Descendants()
             .Where(x => x.Name.LocalName == "Reference")
             .Where(x => (string)x.Attribute("Include") == "Boo.Lang")
             .Remove();
            return document.Declaration + Environment.NewLine + document.Root;
        };
    }
}
