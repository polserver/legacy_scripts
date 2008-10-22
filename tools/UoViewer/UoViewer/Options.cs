using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using System.Xml;
using System.IO;
using Ultima;
using Controls;
using System.Drawing;
using System.Windows.Forms.Design;
using System.ComponentModel;
using System.Collections;

namespace UoViewer
{
    public class Options 
    {
        public Options() 
        {
            FileIndex.LoadMulPath();
        }

        public static void Reload()
        {
            FileIndex.LoadMulPath();
        }

        public static void Save()
        {
            string filepath = AppDomain.CurrentDomain.SetupInformation.ApplicationBase;

            string FileName = Path.Combine(filepath, "Options.xml");

            XmlDocument dom = new XmlDocument();
            XmlDeclaration decl = dom.CreateXmlDeclaration("1.0", "utf-8", null);
            dom.AppendChild(decl);
            XmlElement sr = dom.CreateElement("Options");

            XmlComment comment= dom.CreateComment("ItemSize controls the size of images in items tab");
            sr.AppendChild(comment);
            XmlElement elem = dom.CreateElement("ItemSize");
            elem.SetAttribute("width", ItemShow.ItemSizeWidth.ToString());
            elem.SetAttribute("height", ItemShow.ItemSizeHeight.ToString());
            sr.AppendChild(elem);
            comment = dom.CreateComment("ItemClip images in items tab shrinked or clipped");
            sr.AppendChild(comment);
            elem = dom.CreateElement("ItemClip");
            elem.SetAttribute("active", ItemShow.ItemClip.ToString());
            sr.AppendChild(elem);
            comment = dom.CreateComment("CacheData should mul entries be cached for faster load");
            sr.AppendChild(comment);
            elem = dom.CreateElement("CacheData");
            elem.SetAttribute("active", FileIndex.CacheData.ToString());
            sr.AppendChild(elem);
            comment = dom.CreateComment("Pathsettings");
            sr.AppendChild(comment);
            foreach (DictionaryEntry e in FileIndex.MulPath)
            {
                XmlElement path = dom.CreateElement("Paths");
                path.SetAttribute("key", e.Key.ToString());
                path.SetAttribute("value", e.Value.ToString());

                sr.AppendChild(path);
            }
            dom.AppendChild(sr);
            dom.Save(FileName);
        }
 
        public static void Load()
        {
            Options Options = new Options();

            // Look for filename
            System.Reflection.Assembly theExe = Options.GetType().Assembly;

            string file = theExe.Location;

            string FileName = Path.Combine(Path.GetDirectoryName(file), "Options.xml");

            if (!System.IO.File.Exists(FileName))
                return;

            XmlDocument dom = new XmlDocument();
            dom.Load(FileName);
            XmlElement xOptions = dom["Options"];

            XmlElement itemsize = (XmlElement)xOptions.SelectSingleNode("ItemSize");
            if (itemsize != null)
            {
                ItemShow.ItemSizeWidth = int.Parse(itemsize.GetAttribute("width"));
                ItemShow.ItemSizeHeight = int.Parse(itemsize.GetAttribute("height"));
            }
            XmlElement itemclip = (XmlElement)xOptions.SelectSingleNode("ItemClip");
            if (itemclip != null)
                ItemShow.ItemClip = bool.Parse(itemclip.GetAttribute("active"));

            XmlElement cachedata = (XmlElement)xOptions.SelectSingleNode("CacheData");
            if (cachedata != null)
                FileIndex.CacheData = bool.Parse(cachedata.GetAttribute("active"));

            foreach (XmlElement xPath in xOptions.SelectNodes("Paths"))
            {
                string key;
                string value;
                key = xPath.GetAttribute("key");
                value = xPath.GetAttribute("value");
                FileIndex.MulPath[key] = value;
            }
        }
    }
}