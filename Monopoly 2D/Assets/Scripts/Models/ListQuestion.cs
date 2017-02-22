using UnityEngine;

namespace AssemblyCSharp
{
    public class ListQuestion
    {
        public Question[] ListQuesitions;

        private string ToJson()
        {
            return JsonUtility.ToJson(this);
        }

        private static ListQuestion FromJson(string json)
        {
            return JsonUtility.FromJson<ListQuestion>(json);
        }

        public static ListQuestion GetQuestions()
        {
            TextAsset questions = Resources.Load<TextAsset>("Data/question");
            return ListQuestion.FromJson(questions.text);
        }

        public Question GetRandQuestion()
        {
            return ListQuesitions[(int)Random.Range(0f, ListQuesitions.Length - 1)];
        }
    }
}
